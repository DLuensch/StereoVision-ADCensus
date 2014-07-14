/* ----------------------------------------------------------------------------
 * Robotics Laboratory, Westphalian University of Applied Science
 * ----------------------------------------------------------------------------
 * Project			: 	Stereo Vision Project
 * Revision			: 	1.0
 * Recent changes	: 	18.06.2014	 
 * ----------------------------------------------------------------------------
 * LOG:
 * ----------------------------------------------------------------------------
 * Developer		: 	Dennis Luensch 		(dennis.luensch@gmail.com)
						Tom Marvin Liebelt	(fh@tom-liebelt.de)
						Christian Blesing	(christian.blesing@gmail.com)
 * License 			: 	BSD 
 *
 * Copyright (c) 2014, Dennis Lünsch, Tom Marvin Liebelt, Christian Blesing
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * # Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * # Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * # Neither the name of the {organization} nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ------------------------------------------------------------------------- */

#include <iostream>
#include "stereoprocessor.h"
#include <libconfig.h++>
#include "imageprocessor.h"
#include <cstdlib>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <iomanip>

#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>

using namespace std;
using namespace cv;

bool loadImageList(string file, vector<string> &list)
{
    bool loadSuccess = false;
    FileNode fNode;
    FileStorage fStorage(file, FileStorage::READ);

    if (fStorage.isOpened())
    {
        fNode = fStorage.getFirstTopLevelNode();

        if (fNode.type() == FileNode::SEQ)
        {
            for(FileNodeIterator iterator = fNode.begin(); iterator != fNode.end(); ++iterator)
            {
                list.push_back((string) *iterator);
            }

            loadSuccess = true;
        }
    }

    return loadSuccess;
}

bool loadImages(vector<string> fileList, vector<Mat> &images)
{
    bool emptyImage = false;
    for (int i = 0; i < fileList.size() && !emptyImage; ++i)
    {
        Mat curImg = imread(fileList[i]);
        if (!curImg.empty())
        {
            images.push_back(curImg);
        }
        else
        {
            emptyImage = true;
        }
    }

    return (!emptyImage && images.size() > 0);
}

bool loadQMatrix(string file, Mat &Q)
{
    bool success = false;
    try
    {
        FileStorage fStorage(file.c_str(), FileStorage::READ);
        fStorage["Q"] >> Q;
        fStorage.release();
        success = true;
    }
    catch(Exception ex)
    {
    }

    return success;
}

void createAndSavePointCloud(Mat &disparity, Mat &leftImage, Mat &Q, string filename)
{
    pcl::PointCloud<pcl::PointXYZRGB> pointCloud;

    // Read out Q Values for faster access
    double Q03 = Q.at<double>(0, 3);
    double Q13 = Q.at<double>(1, 3);
    double Q23 = Q.at<double>(2, 3);
    double Q32 = Q.at<double>(3, 2);
    double Q33 = Q.at<double>(3, 3);

    for (int i = 0; i < disparity.rows; i++)
    {
        for (int j = 0; j < disparity.cols; j++)
        {
            // Create a new point
            pcl::PointXYZRGB point;

            // Read disparity
            float d = disparity.at<float>(i, j);
            if ( d <= 0 ) continue; //Discard bad pixels

            // Read color
            Vec3b colorValue = leftImage.at<Vec3b>(i, j);
            point.r = static_cast<int>(colorValue[2]);
            point.g = static_cast<int>(colorValue[1]);
            point.b = static_cast<int>(colorValue[0]);

            // Transform 2D -> 3D and normalise to point
            double x = Q03 + j;
            double y = Q13 + i;
            double z = Q23;
            double w = (Q32 * d) + Q33;
            point.x = -x / w;
            point.y = -y / w;
            point.z = z / w;

            // Put point into the cloud
            pointCloud.points.push_back (point);
        }
    }

    // Resize PCL and save to file
    pointCloud.width = pointCloud.points.size();
    pointCloud.height = 1;
    pcl::io::savePCDFileASCII(filename, pointCloud);
}

int main(int argc, char *argv[])
{
    bool readSuccessfully = false;
    libconfig::Config cfg;

    bool success = false;

    string xmlImages, ymlExtrinsic;
    uint dMin; uint dMax; Size censusWin; float defaultBorderCost;
    float lambdaAD; float lambdaCensus; string savePath; uint aggregatingIterations;
    uint colorThreshold1; uint colorThreshold2; uint maxLength1; uint maxLength2; uint colorDifference;
    float pi1; float pi2; uint dispTolerance; uint votingThreshold; float votingRatioThreshold;
    uint maxSearchDepth; uint blurKernelSize; uint cannyThreshold1; uint cannyThreshold2; uint cannyKernelSize;

    if(argc == 2)
    {

        try
        {
            cfg.readFile(argv[1]);
            readSuccessfully = true;
        }
        catch(const libconfig::FileIOException &fioex)
        {
            cerr << "[ADCensusCV] I/O error while reading file." << endl;
        }
        catch(const libconfig::ParseException &pex)
        {
            cerr << "[ADCensusCV] Parsing error" << endl;
        }


        if(readSuccessfully)
        {
            try
            {
                dMin = (uint) cfg.lookup("dMin");
                dMax = (uint) cfg.lookup("dMax");
                xmlImages = (const char *) cfg.lookup("xmlImages");
                ymlExtrinsic = (const char *) cfg.lookup("ymlExtrinsic");
                censusWin.height = (uint) cfg.lookup("censusWinH");
                censusWin.width = (uint) cfg.lookup("censusWinW");
                defaultBorderCost = (float) cfg.lookup("defaultBorderCost");
                lambdaAD = (float) cfg.lookup("lambdaAD"); // TODO Namen anpassen
                lambdaCensus = (float) cfg.lookup("lambdaCensus");
                savePath = (const char *) cfg.lookup("savePath");
                aggregatingIterations = (uint) cfg.lookup("aggregatingIterations");
                colorThreshold1 = (uint) cfg.lookup("colorThreshold1");
                colorThreshold2 = (uint) cfg.lookup("colorThreshold2");
                maxLength1 = (uint) cfg.lookup("maxLength1");
                maxLength2 = (uint) cfg.lookup("maxLength2");
                colorDifference = (uint) cfg.lookup("colorDifference");
                pi1 = (float) cfg.lookup("pi1");
                pi2 = (float) cfg.lookup("pi2");
                dispTolerance = (uint) cfg.lookup("dispTolerance");
                votingThreshold = (uint) cfg.lookup("votingThreshold");
                votingRatioThreshold = (float) cfg.lookup("votingRatioThreshold");
                maxSearchDepth = (uint) cfg.lookup("maxSearchDepth");
                blurKernelSize = (uint) cfg.lookup("blurKernelSize");
                cannyThreshold1 = (uint) cfg.lookup("cannyThreshold1");
                cannyThreshold2 = (uint) cfg.lookup("cannyThreshold2");
                cannyKernelSize = (uint) cfg.lookup("cannyKernelSize");
            }
            catch(const libconfig::SettingException &ex)
            {
                cerr << "[ADCensusCV] " << ex.what() << endl
                     << "config file format:\n"
                        "dMin(uint)\n"
                        "xmlImages(string)\n"
                        "ymlExtrinsic(string)\n"
                        "censusWinH(uint)\n"
                        "censusWinW(uint)\n"
                        "defaultBorderCost(float)\n"
                        "lambdaAD(float)\n"
                        "lambdaCensus(float)\n"
                        "savePath(string)\n"
                        "aggregatingIterations(uint)\n"
                        "colorThreshold1(uint)\n"
                        "colorThreshold2(uint)\n"
                        "maxLength1(uint)\n"
                        "maxLength2(uint)\n"
                        "colorDifference(uint)\n"
                        "pi1(float)\n"
                        "pi2(float)\n"
                        "dispTolerance(uint)\n"
                        "votingThreshold(uint)\n"
                        "votingRatioThreshold(float)\n"
                        "maxSearchDepth(uint)\n"
                        "blurKernelSize(uint)\n"
                        "cannyThreshold1(uint)\n"
                        "cannyThreshold2(uint)\n"
                        "cannyKernelSize(uint)\n";
                readSuccessfully = false;
            }
        }

        if(readSuccessfully)
        {
            vector<string> fileList;
            vector<Mat> images;
            Mat Q(4, 4, CV_64F);

            boost::filesystem::path dir(savePath);
            boost::filesystem::create_directories(dir);

            bool gotExtrinsic = loadQMatrix(ymlExtrinsic, Q);
            if(loadImageList(xmlImages, fileList))
            {
                if(loadImages(fileList, images))
                {
                    if (images.size() % 2 == 0)
                    {
                        bool error = false;
                        for (int i = 0; i < (images.size() / 2) && !error; ++i)
                        {
                            stringstream file;
                            file << savePath << i;
                            boost::posix_time::ptime start = boost::posix_time::second_clock::local_time();
                            boost::posix_time::ptime end;
                            boost::posix_time::time_duration diff;

                            ImageProcessor iP(0.1);
                            Mat eLeft, eRight;
                            eLeft = iP.unsharpMasking(images[i * 2], "gauss", 3, 1.9, -1);
                            eRight = iP.unsharpMasking(images[i * 2 + 1], "gauss", 3, 1.9, -1);

                            StereoProcessor sP(dMin, dMax, images[i * 2], images[i * 2 + 1], censusWin, defaultBorderCost, lambdaAD, lambdaCensus, file.str(),
                                               aggregatingIterations, colorThreshold1, colorThreshold2, maxLength1, maxLength2,
                                               colorDifference, pi1, pi2, dispTolerance, votingThreshold, votingRatioThreshold,
                                               maxSearchDepth, blurKernelSize, cannyThreshold1, cannyThreshold2, cannyKernelSize);
                            string errorMsg;
                            error = !sP.init(errorMsg);

                            if(!error && sP.compute())
                            {
                                success = true;
                                if(gotExtrinsic)
                                {
                                    Mat disp = sP.getDisparity();

                                    string dispFile = file.str();
                                    dispFile += "_disp.yml";
                                    FileStorage fs(dispFile, FileStorage::WRITE);
                                    fs << "disp" << disp;
                                    fs.release();

                                    file << "_cloud.pcd";
                                    createAndSavePointCloud(disp, images[i * 2], Q, file.str());
                                }
                                else
                                {
                                    cerr << "[ADCensusCV] Could not create point cloud (no extrinsic)!" << endl;
                                }
                            }
                            else
                            {
                                cerr << "[ADCensusCV] " << errorMsg << endl;
                            }

                            end = boost::posix_time::second_clock::local_time();

                            diff = end - start;

                            cout << "Finished computation after " << setw(2) << right <<  setfill('0') << ((int)(diff.total_seconds() / 3600)) << ":"
                                                                   << setw(2) << right <<  setfill('0') << ((int)((diff.total_seconds() / 60) % 60)) << ":"
                                                                   << setw(2) << right <<  setfill('0') << (diff.total_seconds() % 60) << " (" << diff.total_seconds() << "s) !" << endl;

                        }
                    }
                    else
                    {
                        cerr << "[ADCensusCV] Not an even image number!" << endl;
                    }
                }
                else
                {
                    cerr << "[ADCensusCV] Could not read images!" << endl;
                }
            }
            else
            {
                cerr << "[ADCensusCV] Could not read image list!" << endl;
            }
        }
    }
    else
    {
        cerr << "[ADCensusCV] ADCensusBM <config file>" << endl;
    }

    return (success)? EXIT_SUCCESS: EXIT_FAILURE;
}

