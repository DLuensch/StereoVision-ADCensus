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

#include "rectify.h"

void intrinsicExtrinsic::printError(string errorSource, string errorMsg)
{
    cerr << "[ " <<  errorSource << " ] caused a fault: " << errorMsg << endl;
}

bool intrinsicExtrinsic::readRectifyParams(string confPath, string &xmlImages, string &savePath,
                                          string &ymlIntrinsic1, string &ymlIntrinsic2, string &ymlExtrinsics)
{
    bool readSuccess = false;
    libconfig::Config cfg;
    stringstream errorMsg;

    try
    {
        cfg.readFile(confPath.c_str());

        xmlImages = (const char *)cfg.lookup("imagesXml");
        savePath = (const char *)cfg.lookup("savePath");
        ymlIntrinsic1 = (const char *)cfg.lookup("ymlIntrinsic1");
        ymlIntrinsic2 = (const char *)cfg.lookup("ymlIntrinsic2");
        ymlExtrinsics = (const char *)cfg.lookup("ymlExtrinsics");

        readSuccess = true;
    }
    catch(const libconfig::FileIOException &fioex)
    {
        printError("readStereoParams", "I/O error while reading file.");
    }
    catch(const libconfig::ParseException &pex)
    {
        errorMsg << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError();
        printError("readStereoParams", errorMsg.str());
    }
    catch(const libconfig::SettingNotFoundException &nfex)
    {
        printError("readStereoParams", "Error in the configuration file! Missing command or incorrect spelling!"
                   "Necessary parameters: 'imagesXml' (string), 'chessboardWidth' (int), 'chessboardHeight' (int),"
                   " 'subPixSize' (int), 'savePath' (string), 'intrinsic1Yml (string)', 'intrinsic2Yml (string)',"
                   " 'ymlExtrinsics (string)'");
    }

    return readSuccess;
}

bool intrinsicExtrinsic::readMonoParams(string confPath, Size &boardSize, string &xmlImages,
                                          Size &subPixSize, string &savePath)
{
    bool readSuccess = false;
    libconfig::Config cfg;
    stringstream errorMsg;
    int width, height, scale;

    try
    {
        cfg.readFile(confPath.c_str());

        xmlImages = (const char *)cfg.lookup("imagesXml");
        savePath = (const char *)cfg.lookup("savePath");
        width = (int) cfg.lookup("chessboardWidth");
        height = (int) cfg.lookup("chessboardHeight");
        scale = (int) cfg.lookup("subPixSize");

        subPixSize = Size(scale, scale);
        boardSize = Size(width, height);

        readSuccess = true;
    }
    catch(const libconfig::FileIOException &fioex)
    {
        printError("readStereoParams", "I/O error while reading file.");
    }
    catch(const libconfig::ParseException &pex)
    {
        errorMsg << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError();
        printError("readStereoParams", errorMsg.str());
    }
    catch(const libconfig::SettingNotFoundException &nfex)
    {
        printError("readStereoParams", "Error in the configuration file! Missing command or incorrect spelling!"
                   "Necessary parameters: 'imagesXml' (string), 'chessboardWidth' (int), 'chessboardHeight' (int),"
                   " 'subPixSize' (int), 'savePath' (string), 'intrinsic1Yml (string)', 'intrinsic2Yml (string)'");
    }

    return readSuccess;
}

bool intrinsicExtrinsic::loadStereoExtrinsics(string ymlExtrinsics, Mat &R1, Mat &P1, Mat &R2, Mat &P2, Rect &validRoi)
{
    FileStorage fStorage(ymlExtrinsics.c_str(), FileStorage::READ);
    bool readParams = false;
	Rect roi1, roi2;

    if (fStorage.isOpened())
    {
        fStorage["R1"] >> R1;
        fStorage["P1"] >> P1;
        fStorage["R2"] >> R2;
        fStorage["P2"] >> P2;

        fStorage["roi1"] >> roi1;
        fStorage["roi2"] >> roi2;

		Point2i rectCorner1(max(roi1.x, roi2.x), max(roi1.y, roi2.y));

        Point2i rectCorner2(min(roi1.x + roi1.width, roi2.x + roi2.width),
                            min(roi1.y + roi1.height, roi2.y + roi2.height));

        validRoi = Rect(rectCorner1.x, rectCorner1.y,
                 rectCorner2.x - rectCorner1.x, rectCorner2.y - rectCorner1.y);

        readParams = true;

        fStorage.release();
    }

    return readParams;
}

bool intrinsicExtrinsic::loadStereoIntrinsics(string &ymlIntrinsic1, string &ymlIntrinsic2,
                                              Mat &cameraMatrix1, Mat &cameraMatrix2,
                                              Mat &distCoeffs1,  Mat &distCoeffs2)
{
    bool openned;

    FileStorage fStorage1(ymlIntrinsic1.c_str(), FileStorage::READ);
    FileStorage fStorage2(ymlIntrinsic2.c_str(), FileStorage::READ);

    openned = fStorage1.isOpened() && fStorage2.isOpened();

    if (openned)
    {
        fStorage1["Camera Matrix"] >> cameraMatrix1;
        fStorage1["Distortion Coefficients"] >> distCoeffs1;
        fStorage1.release();

        fStorage2["Camera Matrix"] >> cameraMatrix2;
        fStorage2["Distortion Coefficients"] >> distCoeffs2;
        fStorage2.release();
    }

    return openned;
}

bool intrinsicExtrinsic::loadImageList(string file, vector<string> &list)
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

bool intrinsicExtrinsic::loadImages(vector<string> fileList, vector<Mat> &images)
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

bool intrinsicExtrinsic::undistortStereoImages(vector<Mat> &inputImages, vector<Mat> &outputImages,
                                               Mat &cameraMatrix1, Mat &cameraMatrix2,
                                               Mat &distCoeffs1,  Mat &distCoeffs2)
{
    bool undistortSuccess = false;

    if (inputImages.size() % 2 == 0)    // Nur wenn fuer die linke und rechte Kamera gleich viele Bilder existieren.
    {
        for (int i = 0; i < (inputImages.size() / 2); ++i)
        {
            Mat undistort1, undistort2;
            undistort(inputImages[i * 2], undistort1, cameraMatrix1, distCoeffs1);
            undistort(inputImages[i * 2 + 1], undistort2, cameraMatrix2, distCoeffs2);

            outputImages.push_back(undistort1);
            outputImages.push_back(undistort2);
        }

        undistortSuccess = true;
    }
    else
    {
        printError("undistortStereoImages", "Odd number of images! Even number of images required!");
    }

    return undistortSuccess;

}
