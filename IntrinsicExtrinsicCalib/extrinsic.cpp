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

#include "intrinsicextrinsiccalib.h"
#include <boost/filesystem.hpp>
#include <cstdlib>

//#define DEBUG

using namespace IntrinsicExtrinsicCalib;

#include <sstream>

int main(int argc, char** argv)
{
    bool ok = false;
    Size boardSize, subPixSize;
    string xmlImages, savePath, ymlIntrinsic1, ymlIntrinsic2;
    vector<string> fileList;
    vector<Mat> images;
    vector<Mat> undistortedImages;
    Mat cameraMatrix1, cameraMatrix2, distCoeffs1, distCoeffs2;
    vector<vector<Point2f> > cornersCam1, cornersCam2;
    vector<Point3f> idealCorners;
    Mat R, T;
    Mat noDist = Mat::zeros(5,1, CV_32F);
    Size imageSize;
    vector<Mat> rvecs1, tvecs1, rvecs2, tvecs2;
    Mat R1, R2, P1, P2, Q;
    Rect validRoi[2];
    float squareSize;

    ok = readStereoParams(string(argv[1]), boardSize, squareSize, xmlImages, subPixSize, savePath, ymlIntrinsic1, ymlIntrinsic2);

    if (ok)
    {
        cout << "[Extrinsic] load image list!" << endl;
        ok = loadImageList(xmlImages, fileList);

        if (!ok)
            cout << "[Extrinsic] error! could not load image list!" << endl;
    }

    if (ok)
    {
        cout << "[Extrinsic] load images!" << endl;
        ok = loadImages(fileList, images);

        if (!ok)
            cout << "[Extrinsic] error! could not load images!" << endl;
    }

    if (ok)
    {
        boost::filesystem::path dir(savePath);
        boost::filesystem::create_directories(dir);

        cout << "[Extrinsic] load intrinsics and undistort images!" << endl;
        loadStereoIntrinsics(ymlIntrinsic1, ymlIntrinsic2,
                             cameraMatrix1, cameraMatrix2,distCoeffs1, distCoeffs2);

        ok = undistortStereoImages(images, undistortedImages,
                                   cameraMatrix1, cameraMatrix2,distCoeffs1, distCoeffs2);

        if (!ok)
            cout << "[Extrinsic] error! could not load intrinsics and undistort images!" << endl;
    }

    if (ok)
    {
        cout << "[Extrinsic] find corners!" << endl;

        ok = findCornersStereo(boardSize, subPixSize, undistortedImages, cornersCam1, cornersCam2);

        if (!ok)
            cout << "[Extrinsic] error! could not find corners!" << endl;
    }


    if (ok)
    {
        rvecs1.resize(cornersCam1.size());
        tvecs1.resize(cornersCam1.size());
        rvecs2.resize(cornersCam1.size());
        tvecs2.resize(cornersCam1.size());

        cout << "[Extrinsic] calculate extrinsic with " << cornersCam1.size() << " pairs!" << endl;
        calcBoardCornerPositions(boardSize, squareSize, idealCorners);

        for (int i = 0; i < cornersCam1.size(); i++)
        {
            solvePnP(idealCorners, cornersCam1[i], cameraMatrix1, noDist, rvecs1[i], tvecs1[i]);
            solvePnP(idealCorners, cornersCam2[i], cameraMatrix2, noDist, rvecs2[i], tvecs2[i]);
        }

        ok = calculateRT(tvecs1, tvecs2, R, T);

        if (!ok)
            cout << "[Extrinsic] error! could not calculate extrinsic!" << endl;
    }

    if (ok)
    {
        cout << "[Extrinsic] save extrinsic parameters!" << endl;
        imageSize = ((Mat)images[0]).size();
        stereoRectify(cameraMatrix1, noDist,
                      cameraMatrix2, noDist,
                      imageSize, R, T, R1, R2, P1, P2, Q,
                      CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

        string ymlPath(savePath);
        ymlPath += "extrinsics.yml";

        FileStorage fS(ymlPath.c_str(), FileStorage::WRITE);

        fS << "R" << R << "T" << T << "R1" << R1 << "R2" << R2 << "P1" << P1 << "P2" << P2 << "Q" << Q
           << "roi1" << validRoi[0] << "roi2" << validRoi[1];

        fS.release();

        Mat rmap[2][2];
        initUndistortRectifyMap(cameraMatrix1, noDist, R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
        initUndistortRectifyMap(cameraMatrix2, noDist, R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

#ifdef DEBUG
        // Show rectified images
        bool isVerticalStereo = fabs(P2.at<double>(1, 3)) > fabs(P2.at<double>(0, 3));

        Mat canvas;
        double sf;
        int w, h;
        if( !isVerticalStereo )
        {
            sf = 600./MAX(imageSize.width, imageSize.height);
            w = cvRound(imageSize.width*sf);
            h = cvRound(imageSize.height*sf);
            canvas.create(h, w*2, CV_8UC3);
        }
        else
        {
            sf = 300./MAX(imageSize.width, imageSize.height);
            w = cvRound(imageSize.width*sf);
            h = cvRound(imageSize.height*sf);
            canvas.create(h*2, w, CV_8UC3);
        }

        for(int i = 0; i < (undistortedImages.size() / 2); i++)
        {
            for(int k = 0; k < 2; k++)
            {
                Mat img = undistortedImages[i*2+k], rimg;
                remap(img, rimg, rmap[k][0], rmap[k][1], CV_INTER_LINEAR);

                stringstream filename;

                filename << savePath << ((k == 0)? "1": "2") << "_image_" << i << ".jpg";

                imwrite(filename.str(), rimg);

                Mat canvasPart = !isVerticalStereo ? canvas(Rect(w*k, 0, w, h)) : canvas(Rect(0, h*k, w, h));
                resize(rimg, canvasPart, canvasPart.size(), 0, 0, CV_INTER_AREA);

                    Rect vroi(cvRound(validRoi[k].x*sf), cvRound(validRoi[k].y*sf),
                              cvRound(validRoi[k].width*sf), cvRound(validRoi[k].height*sf));
                    rectangle(canvasPart, vroi, Scalar(0,0,255), 3, 8);

            }

            if( !isVerticalStereo )
                for(int j = 0; j < canvas.rows; j += 16 )
                    line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
            else
                for(int j = 0; j < canvas.cols; j += 16 )
                    line(canvas, Point(j, 0), Point(j, canvas.rows), Scalar(0, 255, 0), 1, 8);
            imshow("rectified", canvas);
            char c = (char)waitKey();
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }
#endif

        Point2i rectCorner1(max(validRoi[0].x, validRoi[1].x), max(validRoi[0].y, validRoi[1].y));

        Point2i rectCorner2(min(validRoi[0].x + validRoi[0].width, validRoi[1].x + validRoi[1].width),
                            min(validRoi[0].y + validRoi[0].height, validRoi[1].y + validRoi[1].height));

        Rect roi(rectCorner1.x, rectCorner1.y,
                 rectCorner2.x - rectCorner1.x, rectCorner2.y - rectCorner1.y);

        for(int i = 0; i < (undistortedImages.size() / 2); i++)
        {
            for(int k = 0; k < 2; k++)
            {
                Mat img = undistortedImages[i*2+k], remapImg, rectImg;
                remap(img, remapImg, rmap[k][0], rmap[k][1], CV_INTER_LINEAR);

                stringstream filename;

                filename << savePath << "image" << i << ((k == 0)? "_1": "_2") << ".jpg";

                rectImg = remapImg(roi);
                imwrite(filename.str(), rectImg);
            }
        }
    }

    return EXIT_SUCCESS;
}
