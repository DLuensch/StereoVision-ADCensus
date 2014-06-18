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

#include "intrinsicextrinsiccalib.h"
#include <boost/filesystem.hpp>

using namespace IntrinsicExtrinsicCalib;

void saveImages(vector<Mat> &imagesMarkedPattern, vector<Mat> &undistortedImages, string savePath)
{
    for (int i = 0; i < imagesMarkedPattern.size(); i++)
    {
        stringstream filename, filenameUndistorted;

        filename << savePath << i << "_image" << ".png";
        filenameUndistorted << savePath << i << "_undistored_image" << ".png";

        imwrite(filename.str(), imagesMarkedPattern[i]);
        imwrite(filenameUndistorted.str(), undistortedImages[i]);
    }
}

int main(int argc, char** argv)
{
    bool ok = false;
    Size boardSize, subPixSize;
    string xmlImages, savePath;
    vector<string> fileList;
    vector<Mat> images, undistortedImages;
    vector<vector<Point2f> > cornersCam;
    Mat cameraMatrix, distCoeffs;
    vector<Mat> rvecs, tvecs;
    vector<float> reprojErrs;
    double totalAvgErr;
    int flags;
    bool autoMode;


    ok = readMonoParams(string(argv[1]), boardSize, xmlImages, subPixSize, savePath, autoMode, flags);

    if (ok)
    {
        cout << "[Intrinsic] load image list!" << endl;
        ok = loadImageList(xmlImages, fileList);

        if (!ok)
            cout << "[Intrinsic] error! could not load image list!" << endl;
    }

    if (ok)
    {
        cout << "[Intrinsic] load images!" << endl;
        ok = loadImages(fileList, images);

        if (!ok)
            cout << "[Intrinsic] error! could not load images!" << endl;
    }

    if (ok)
    {
        boost::filesystem::path dir(savePath);
        boost::filesystem::create_directories(dir);
        cout << "[Intrinsic] find corners!" << endl;
        ok = findCornersMono(boardSize, subPixSize, images, cornersCam);

        if (!ok)
            cout << "[Intrinsic] error! could not find corners!" << endl;
    }

    if (ok)
    {
        cout << "[Intrinsic] calculate intrinsic!" << endl;
        if(!autoMode)
        {
        ok = calculateIntrinsic(boardSize, ((Mat)images[0]).size(), cameraMatrix, distCoeffs, cornersCam,
                                rvecs, tvecs, reprojErrs, totalAvgErr, flags);

        if (!ok)
            cout << "[Intrinsic] error! could not calculate intrinsic!" << endl;
        }
        else
        {
            vector<int> bestParams;
            ParamCombination paramCombi;
            bestParams = findBestCalibParams(paramCombi, boardSize, ((Mat)images[0]).size(), cameraMatrix, distCoeffs, cornersCam, true, savePath);

            paramCombi.createConfig(xmlImages, savePath, boardSize.width, boardSize.height, subPixSize.width, bestParams);
        }
    }

    if (ok)
    {
        cout << "[Intrinsic] save intrinsic parameters!" << endl;
        string ymlPath(savePath);
        ymlPath += "intrinsic.yml";
        FileStorage fs(ymlPath.c_str(), FileStorage::WRITE);
        fs << "Camera Matrix" << cameraMatrix << "Distortion Coefficients" << distCoeffs;
        fs.release();

        for (int i = 0; i < images.size(); i++)
        {
            Mat undistorted;

            undistort(images[i], undistorted, cameraMatrix, distCoeffs);
            undistortedImages.push_back(undistorted);

            drawChessboardCorners(images[i], boardSize, Mat(cornersCam[i]), true);
        }
        saveImages(images, undistortedImages, savePath);

    }

    return 0;
}
