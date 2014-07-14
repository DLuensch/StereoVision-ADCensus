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
#include <opencv2/opencv.hpp>
#include <libconfig.h++>

using namespace std;
using namespace cv;
using namespace libconfig;

bool loadFileList(string file, vector<string> &list)
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

bool loadDisparities(vector<string> fileList, vector<Mat> &disps)
{
    bool emptyDisp = false;
    FileStorage fStorage;
    for (int i = 0; i < fileList.size() && !emptyDisp; ++i)
    {
        Mat curDisp;
        fStorage.open(fileList[i], FileStorage::READ);
        fStorage["disp"] >> curDisp;
        fStorage.release();

        if (!curDisp.empty())
        {
            disps.push_back(curDisp);
        }
        else
        {
            emptyDisp = true;
        }
    }

    return (!emptyDisp && disps.size() > 0);
}

template <typename T>
void convertDisparity(const Mat &disp, Mat &grayDisp, bool stretch)
{
    Size imgSize = disp.size();
    Mat output(imgSize, CV_8UC3);
    T min, max;

    if(stretch)
    {
        min = std::numeric_limits<T>::max();
        max = 0;
        for(size_t h = 0; h < imgSize.height; h++)
        {
            for(size_t w = 0; w < imgSize.width; w++)
            {
                T disparity = disp.at<T>(h, w);

                if(disparity < min && disparity >= 0)
                    min = disparity;
                else if(disparity > max)
                    max = disparity;
            }
        }
    }

    for(size_t h = 0; h < imgSize.height; h++)
    {
        for(size_t w = 0; w < imgSize.width; w++)
        {
            Vec3b color;
            T disparity = disp.at<T>(h, w);

            if (disparity >= 0)
            {
                if(stretch)
                    disparity = (255 / (max - min)) * (disparity - min);

                color[0] = (uchar)disparity;
                color[1] = (uchar)disparity;
                color[2] = (uchar)disparity;

            }
            else
            {
                color[0] = 0;
                color[1] = 0;
                color[2] = 0;
            }

            output.at<Vec3b>(h, w) = color;
        }
    }

    grayDisp = output.clone();
}


int main(int argc, char *argv[])
{
    vector<vector<string> > cameraFiles;
    vector<float> scaleFactors;

    vector<vector<Mat> > images;
    vector<vector<Mat> > images3D;
    vector<vector<Mat> > disparities;
    vector<vector<Mat> > disparitiesGray;
    vector<Mat> qMats;

    if(argc == 2)
    {
        bool readSuccessfully = false;
        Config cfg;

        try
        {
            cfg.readFile(argv[1]);
            readSuccessfully = true;
        }
        catch(const FileIOException &fioex)
        {
            cerr << "[evalDisp] I/O error while reading config file." << endl;
        }
        catch(const ParseException &pex)
        {
            cerr << "[evalDisp] Parsing error in config file." << endl;
        }


        if(readSuccessfully)
        {
            const Setting &root = cfg.getRoot();

            try
            {
              const Setting &cameras = root["cameras"];
              int count = cameras.getLength();
              cameraFiles.resize(count);
              scaleFactors.resize(count);
              for(int i = 0; i < count; ++i)
              {
                  const Setting &camera = cameras[i];
                  cameraFiles[i].resize(4);

                  if(!(camera.lookupValue("cameraName", cameraFiles[i][0])
                       && camera.lookupValue("extrinsicFile", cameraFiles[i][1])
                       && camera.lookupValue("imagesXML", cameraFiles[i][2])
                       && camera.lookupValue("dispsXML", cameraFiles[i][3])
                       && camera.lookupValue("scaleFactor", scaleFactors[i])))
                  {
                      readSuccessfully = false;
                  }
              }
            }
            catch(const SettingNotFoundException &nfex)
            {
              cerr << "[evalDisp] No camera files found." << endl;
              readSuccessfully = false;
            }
        }

        if(readSuccessfully)
        {
            try
            {
                int camCount = cameraFiles.size();
                qMats.resize(camCount);
                images.resize(camCount);
                images3D.resize(camCount);
                disparities.resize(camCount);
                disparitiesGray.resize(camCount);

                for(int camNo = 0; camNo < cameraFiles.size(); camNo++)
                {
                    FileStorage fStorage(cameraFiles[camNo][1], FileStorage::READ);
                    fStorage["Q"] >> qMats[camNo];
                    fStorage.release();

                    vector<string> fileList;
                    readSuccessfully = false;

                    if(loadFileList(cameraFiles[camNo][2], fileList)) // Load image filenames
                    {
                        readSuccessfully = loadImages(fileList, images[camNo]);
                    }

                    fileList.clear();

                    if(loadFileList(cameraFiles[camNo][3], fileList)) // Load disparity filenames
                    {
                        readSuccessfully &= loadDisparities(fileList, disparities[camNo]);
                    }

                    readSuccessfully &= (images[camNo].size() == disparities[camNo].size());

                    if(readSuccessfully)
                    {
                        scaleFactors[camNo] = (scaleFactors[camNo] > 0 && scaleFactors[camNo] <= 1)? scaleFactors[camNo]: 1;
                        images3D[camNo].resize(images[camNo].size());
                        disparitiesGray[camNo].resize(images[camNo].size());

                        for(int i = 0; i < images[camNo].size(); i++)
                        {
                            Mat image3D;
                            reprojectImageTo3D(disparities[camNo][i], image3D, qMats[camNo]);

                            Mat resizedImage, resizedDispMap;
                            resize(images[camNo][i], resizedImage, Size(), scaleFactors[camNo], scaleFactors[camNo]);
                            resize(disparities[camNo][i], resizedDispMap, Size(), scaleFactors[camNo], scaleFactors[camNo]);
                            images[camNo][i] = resizedImage.clone();
                            disparities[camNo][i] = resizedDispMap.clone();

                            resize(image3D, images3D[camNo][i], Size(), scaleFactors[camNo], scaleFactors[camNo]);

                            if(disparities[camNo][i].type() == CV_32F)
                                convertDisparity<float>(disparities[camNo][i], disparitiesGray[camNo][i], true);
                            else
                                convertDisparity<uchar>(disparities[camNo][i], disparitiesGray[camNo][i], true);
                        }
                    }
                }
            }
            catch(Exception ex)
            {
                cerr << "[evalDisp] " << ex.msg << " in " << ex.func << endl;
                readSuccessfully = false;
            }
        }


        if(readSuccessfully)
        {
            Mat imageWMarker;
            char key = 0;
            int camNo = 0;
            int capNo = 0;
            bool useImg = true;
            Size imgSize = images[camNo][capNo].size();
            int posX = imgSize.width / 2;
            int posY = imgSize.height / 2;

            cout << endl << "[evalDisp] Keys:" << endl
                 << "w-a-s-d = cross position" << endl
                 << "c       = toggle between disparity and image" << endl
                 << "b-n     = previous/next capture" << endl
                 << "h-j     = previous/next camera" << endl
                 << "q-ESC   = quit" << endl;

            do
            {
                key = waitKey();

                switch (key)
                {
                    case 'a':
                        posX = (posX - 2 < 0)? 0: posX - 2;
                        break;
                    case 'd':
                        posX = (posX + 2 >= imgSize.width)? imgSize.width - 1: posX + 2;
                        break;
                    case 'w':
                        posY = (posY - 2 < 0)? 0: posY - 2;
                        break;
                    case 's':
                        posY = (posY + 2 >= imgSize.height)? imgSize.height - 1: posY + 2;
                        break;
                    case 'n':
                        capNo = (capNo + 1 >= images[camNo].size())? images[camNo].size() - 1: capNo + 1;
                        break;
                    case 'b':
                        capNo = (capNo - 1 < 0)? 0: capNo - 1;
                        break;
                    case 'c':
                        useImg = !useImg;
                        break;
                    case 'j':
                        camNo = (camNo + 1 >= images.size())? images.size() - 1: camNo + 1;
                        imgSize = images[camNo][capNo].size();
                        posX = imgSize.width / 2;
                        posY = imgSize.height / 2;
                        destroyAllWindows();
                        break;
                    case 'h':
                        camNo = (camNo - 1 < 0)? 0: camNo - 1;
                        imgSize = images[camNo][capNo].size();
                        posX = imgSize.width / 2;
                        posY = imgSize.height / 2;
                        destroyAllWindows();
                        break;

                }

                imageWMarker = (useImg)? images[camNo][capNo].clone(): disparitiesGray[camNo][capNo].clone();

                line(imageWMarker, Point(posX - 4,posY), Point(posX + 4,posY), CV_RGB(0,255,0));
                line(imageWMarker, Point(posX,posY - 4), Point(posX,posY + 4), CV_RGB(0,255,0));

                float disp;

                if(disparities[camNo][capNo].type() == CV_32F)
                    disp = disparities[camNo][capNo].at<float>(posY, posX);
                else
                    disp = disparities[camNo][capNo].at<uchar>(posY, posX);

                float vec3DAbs;
                if(disp > 0)
                {
                    Point3f vec3D = images3D[camNo][capNo].at<Point3f>(posY, posX);
                    vec3DAbs = sqrt(pow(vec3D.x, 2) +  pow(vec3D.y, 2) + pow(vec3D.z, 2));
                }
                else
                {
                    vec3DAbs = -1;
                }


                stringstream disparity;
                disparity << "Disparity: " << disp << ", "
                          << "Distance: " << vec3DAbs << "cm";
                putText(imageWMarker, disparity.str(), Point(30, 30), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 0));

                imshow(cameraFiles[camNo][0], imageWMarker);
            }
            while (key != 27 && key != 'q'); // waitKey-Wert: ESC und q

        }

    }
    else
    {
        cout << "Usage: evalDisp <config file>" << endl;
    }
    return 0;
}

