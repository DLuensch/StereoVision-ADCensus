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
#include <boost/filesystem.hpp>

using namespace intrinsicExtrinsic;

#include <string>

int main(int argc, char** argv)
{
    bool ok = false;
    string xmlImages, savePath, ymlIntrinsic1, ymlIntrinsic2, ymlExtrinsics;
    vector<string> fileList;
    vector<Mat> images;
    vector<Mat> undistortedImages;
    Mat cameraMatrix1, cameraMatrix2, distCoeffs1, distCoeffs2;
    Mat noDist = Mat::zeros(5,1, CV_32F);
    Size imageSize;
    Mat R1, R2, P1, P2;
    Rect validRoi;

    if (argc >= 2)
    {
        cout << "[ImageRectify] read params!" << endl;
        ok = readRectifyParams(string(argv[1]), xmlImages, savePath, ymlIntrinsic1, ymlIntrinsic2, ymlExtrinsics);

        if (!ok)
            cout << "[ImageRectify] error! could not read params!" << endl;
    }

    if (ok)
    {
        cout << "[ImageRectify] load image list!" << endl;
        ok = loadImageList(xmlImages, fileList);

        if (!ok)
            cout << "[ImageRectify] error! could not load image list!" << endl;
    }

    if (ok)
    {
        cout << "[ImageRectify] load images!" << endl;
        ok = loadImages(fileList, images);

        if (!ok)
            cout << "[ImageRectify] error! could not load images!" << endl;
    }

    if (ok)
    {
        imageSize.height = ((Mat)images.at(0)).rows;
        imageSize.width = ((Mat)images.at(0)).cols;
    }

    if (ok)
    {
        cout << "[ImageRectify] load intrinsics!" << endl;
        ok = loadStereoIntrinsics(ymlIntrinsic1, ymlIntrinsic2,
                             cameraMatrix1, cameraMatrix2, distCoeffs1, distCoeffs2);

        if (!ok)
            cout << "[ImageRectify] error! could not load intrinsics!" << endl;
    }

    if (ok)
    {
        cout << "[ImageRectify] load extrinsics!" << endl;
        ok = loadStereoExtrinsics(ymlExtrinsics, R1, P1, R2, P2, validRoi);

        if (!ok)
            cout << "[ImageRectify] error! could not load extrinsics!" << endl;
    }

    if (ok)
    {
        cout << "[ImageRectify] undistort images!" << endl;
        ok = undistortStereoImages(images, undistortedImages,
                                   cameraMatrix1, cameraMatrix2, distCoeffs1, distCoeffs2);

        if (!ok)
            cout << "[ImageRectify] error! could not undistort images!" << endl;
    }

    // Remap and save
    if (ok)
    {
        Mat rmap[2][2];
        initUndistortRectifyMap(cameraMatrix1, noDist, R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
        initUndistortRectifyMap(cameraMatrix2, noDist, R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);
        int imgCount = undistortedImages.size() / 2;

		boost::filesystem::path dir(savePath);
        boost::filesystem::create_directories(dir);

        for(int i = 0; i < imgCount; i++)
        {            
            cout << "[ImageRectify] rectify image " << (i + 1) << " of " << imgCount << endl;

            for(int k = 0; k < 2; k++)
            {
                Mat img = undistortedImages[i*2+k], remapImg, rectImg;
                remap(img, remapImg, rmap[k][0], rmap[k][1], CV_INTER_LINEAR);

                stringstream filename;

                filename << savePath << "image" << i << "_" << ((k == 0)? "1": "2") << ".jpg";

                rectImg = remapImg(validRoi);
                imwrite(filename.str(), rectImg);
            }
        }
    }

    return 0;
}
