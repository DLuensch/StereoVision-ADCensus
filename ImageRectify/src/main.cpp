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
