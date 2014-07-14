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

void IntrinsicExtrinsicCalib::printError(string errorSource, string errorMsg)
{
    cerr << "[ " <<  errorSource << " ] caused a fault: " << errorMsg << endl;
}

bool IntrinsicExtrinsicCalib::readStereoParams(string confPath, Size &boardSize, float &squareSize, string &xmlImages,
                                          Size &subPixSize, string &savePath,
                                          string &ymlIntrinsic1, string &ymlIntrinsic2)
{
    bool readSuccess = false;
    libconfig::Config cfg;
    int width, height, scale;

    try
    {
        cfg.readFile(confPath.c_str());

        xmlImages = (const char *)cfg.lookup("imagesXml");
        savePath = (const char *)cfg.lookup("savePath");
        ymlIntrinsic1 = (const char *)cfg.lookup("ymlIntrinsic1");
        ymlIntrinsic2 = (const char *)cfg.lookup("ymlIntrinsic2");
        width = (int) cfg.lookup("chessboardWidth");
        height = (int) cfg.lookup("chessboardHeight");
        scale = (int) cfg.lookup("subPixSize");
        squareSize = (float) cfg.lookup("squareSize");

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
        printError("readStereoParams", "Parse error");
    }
    catch(const libconfig::SettingNotFoundException &nfex)
    {
        printError("readStereoParams", "Error in the configuration file! Missing command or incorrect spelling!"
                   "Necessary parameters: 'imagesXml' (string), 'chessboardWidth' (int), 'chessboardHeight' (int),"
                   " 'subPixSize' (int), 'savePath' (string), 'intrinsic1Yml (string)', 'intrinsic2Yml (string)'");
    }

    return readSuccess;
}

bool IntrinsicExtrinsicCalib::readMonoParams(string confPath, Size &boardSize, string &xmlImages,
                                          Size &subPixSize, string &savePath, bool &autoMode, int &flags)
{
    bool readSuccess = false;
    libconfig::Config cfg;
    int width, height, scale;

    try
    {
        cfg.readFile(confPath.c_str());

        xmlImages = (const char *)cfg.lookup("imagesXml");
        savePath = (const char *)cfg.lookup("savePath");
        width = (int) cfg.lookup("chessboardWidth");
        height = (int) cfg.lookup("chessboardHeight");
        scale = (int) cfg.lookup("subPixSize");
        autoMode = (bool) cfg.lookup("autoCalibrate");

        subPixSize = Size(scale, scale);
        boardSize = Size(width, height);

        if (!autoMode)
        {
            flags = (((bool)cfg.lookup("FIX_PRINCIPAL_POINT")) ? CV_CALIB_FIX_PRINCIPAL_POINT : 0);
            flags += (((bool)cfg.lookup("FIX_ASPECT_RATIO")) ? CV_CALIB_FIX_ASPECT_RATIO : 0);
            flags += (((bool)cfg.lookup("ZERO_TANGENT_DIST")) ? CV_CALIB_ZERO_TANGENT_DIST : 0);
            flags += (((bool)cfg.lookup("RATIONAL_MODEL")) ? CV_CALIB_RATIONAL_MODEL : 0);
            flags += (((bool)cfg.lookup("FIX_K1")) ? CV_CALIB_FIX_K1 : 0);
            flags += (((bool)cfg.lookup("FIX_K2")) ? CV_CALIB_FIX_K2 : 0);
            flags += (((bool)cfg.lookup("FIX_K3")) ? CV_CALIB_FIX_K3 : 0);
            flags += (((bool)cfg.lookup("FIX_K4")) ? CV_CALIB_FIX_K4 : 0);
            flags += (((bool)cfg.lookup("FIX_K5")) ? CV_CALIB_FIX_K5 : 0);
            flags += (((bool)cfg.lookup("FIX_K6")) ? CV_CALIB_FIX_K6 : 0);
        }

        readSuccess = true;
    }
    catch(const libconfig::FileIOException &fioex)
    {
        printError("readMonoParams", "I/O error while reading file.");
    }
    catch(const libconfig::ParseException &pex)
    {
        printError("readMonoParams", "Parse error");
    }
    catch(const libconfig::SettingNotFoundException &nfex)
    {
        printError("readMonoParams", "Error in the configuration file! Missing command or incorrect spelling!"
                   "Necessary parameters: 'imagesXml' (string), 'chessboardWidth' (int), 'chessboardHeight' (int),"
                   "'subPixSize' (int), 'savePath' (string), 'intrinsic1Yml (string)', 'intrinsic2Yml' (string),"
                   "'autoCalibrate' (bool), 'FIX_PRINCIPAL_POINT' (bool), 'FIX_ASPECT_RATIO' (bool), "
                   "'ZERO_TANGENT_DIST' (bool), 'RATIONAL_MODEL' (bool), 'FIX_K1' (bool), 'FIX_K2' (bool),"
                   "'FIX_K3' (bool), 'FIX_K4' (bool), 'FIX_K5' (bool), 'FIX_K6' (bool)");
    }

    return readSuccess;
}

void IntrinsicExtrinsicCalib::loadStereoIntrinsics(string &ymlIntrinsic1, string &ymlIntrinsic2,
                                              Mat &cameraMatrix1, Mat &cameraMatrix2,
                                              Mat &distCoeffs1,  Mat &distCoeffs2)
{
    FileStorage fStorage1(ymlIntrinsic1.c_str(), FileStorage::READ);
    FileStorage fStorage2(ymlIntrinsic2.c_str(), FileStorage::READ);

    fStorage1["Camera Matrix"] >> cameraMatrix1;
    fStorage1["Distortion Coefficients"] >> distCoeffs1;
    fStorage1.release();

    fStorage2["Camera Matrix"] >> cameraMatrix2;
    fStorage2["Distortion Coefficients"] >> distCoeffs2;
    fStorage2.release();
}

bool IntrinsicExtrinsicCalib::loadImageList(string file, vector<string> &list)
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

bool IntrinsicExtrinsicCalib::loadImages(vector<string> fileList, vector<Mat> &images)
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

bool IntrinsicExtrinsicCalib::undistortStereoImages(vector<Mat> &inputImages, vector<Mat> &outputImages,
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

bool IntrinsicExtrinsicCalib::findCornersStereo(Size boardSize, Size subPixSize, vector<Mat> &images,
                                           vector<vector<Point2f> > &cornersCam1,
                                           vector<vector<Point2f> > &cornersCam2)
{
    bool foundSuccess = false;
    vector<Point2f> corners1, corners2;
    bool patternFound1, patternFound2;
    Mat viewGray1, viewGray2;
    int imageCount = 0;
    vector<Mat> newImageOrder;


    if (images.size() % 2 == 0)    // Nur wenn fuer die linke und rechte Kamera gleich viele Bilder existieren.
    {
        int i;
        #pragma omp parallel default (shared) private(i, patternFound1, patternFound2, corners1, corners2, \
                                                      viewGray1, viewGray2) num_threads(omp_get_max_threads())
        #pragma omp for schedule(static)
        for (i = 0; i < (images.size() / 2); ++i)
        {
            patternFound1 = findChessboardCorners(images[i * 2], boardSize, corners1,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);
            patternFound2 = findChessboardCorners(images[i * 2 + 1], boardSize, corners2,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);

            if (patternFound1 && patternFound2)
            {
                vector<Point2f> subPixCorners1(corners1);
                vector<Point2f> subPixCorners2(corners2);

                cvtColor(images[i * 2], viewGray1, CV_BGR2GRAY);
                cvtColor(images[i * 2 + 1], viewGray2, CV_BGR2GRAY);

                cornerSubPix(viewGray1, subPixCorners1, subPixSize,
                             Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
                cornerSubPix(viewGray2, subPixCorners2, subPixSize,
                             Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

                #pragma omp critical
                {
                    cornersCam1.push_back(subPixCorners1);
                    cornersCam2.push_back(subPixCorners2);
                    newImageOrder.push_back(images[i * 2]);
                    newImageOrder.push_back(images[i * 2 + 1]);
                }
            }

            #pragma omp critical
            {
                imageCount++;
                cout << "[findCornersStereo] progress: " << ((int)(((double)imageCount / (images.size() / 2)) * 100)) << "%" << endl;
            }
        }

        images.clear();

        images = newImageOrder;

        foundSuccess = true;
    }
    else
    {
        printError("findCornersStereo", "Odd number of images! Even number of images required!");
    }

    if (cornersCam1.size() == 0)
    {
        printError("findCornersStereo", "No pattern found!");
        foundSuccess = false;
    }

    return foundSuccess;
}

void IntrinsicExtrinsicCalib::calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f>& corners)
{
    for( int i = 0; i < boardSize.height; ++i )
        for( int j = 0; j < boardSize.width; ++j )
            corners.push_back(Point3f(float( j*squareSize ), float( i*squareSize ), 0));
}

void IntrinsicExtrinsicCalib::calcMean(vector<Mat> &points, Mat &mean)
{
    mean = Mat::zeros(3, 1, CV_64F);

    for (int i = 0; i < points.size(); ++i)
    {
        mean += points[i];
    }

    if (points.size() > 0)
    {
        mean = (1.0 / points.size()) * mean;
    }
}

bool IntrinsicExtrinsicCalib::calculateRT(vector<Mat> &tvecs1, vector<Mat> &tvecs2,
                                     Mat &R, Mat &T)
{
    bool success = false;
    Mat meanCam1 = Mat::zeros(3, 1, CV_64F), meanCam2;
    Mat covariance = Mat::zeros(3, 3, CV_64F);
    Mat A;
    Mat E = Mat::eye(3, 3, CV_64F);
    double maxEigenvalue;
    int maxEigenValueIndex = 0;

    Eigen::Matrix4d qSigma;
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix4d> eigenSolver;
    Eigen::Vector4d eigenvalues, maxEigenvector;
    Eigen::Quaterniond q;
    Eigen::Matrix3d rotMat;

    calcMean(tvecs1, meanCam1);
    calcMean(tvecs2, meanCam2);


    if (tvecs1.size() == tvecs2.size())
    {
        for (int i = 0; i < tvecs1.size(); ++i)
        {
            Mat cam1_3d = (Mat_<double>(3,1) << tvecs1[i].at<double>(0,0),
                                                tvecs1[i].at<double>(1,0),
                                                tvecs1[i].at<double>(2,0));

            Mat cam2_3d = (Mat_<double>(3,1) << tvecs2[i].at<double>(0,0),
                                                tvecs2[i].at<double>(1,0),
                                                tvecs2[i].at<double>(2,0));

            covariance += (cam1_3d - meanCam1) * ((cam2_3d - meanCam2).t());
        }

        A = covariance - covariance.t();

        qSigma(0,0) = trace(covariance)[0];
        qSigma(0,1) = A.at<double>(1,2);
        qSigma(0,2) = A.at<double>(2,0);
        qSigma(0,3) = A.at<double>(0,1);

        qSigma(1,0) = A.at<double>(1,2);
        qSigma(2,0) = A.at<double>(2,0);
        qSigma(3,0) = A.at<double>(0,1);

        Mat tempM = covariance + covariance.t() - (trace(covariance)[0] * E);

        for (int i = 1; i < 4; ++i)
        {
            for (int j = 1; j < 4; ++j)
            {
                qSigma(i,j) = tempM.at<double>(i - 1,j - 1);

            }
        }

        eigenSolver.compute(qSigma);
        eigenvalues = eigenSolver.eigenvalues();
        maxEigenvalue = eigenvalues(0);

        for (int i = 1; i < 4; ++i)
        {
            if (maxEigenvalue < eigenvalues(i))
            {
                maxEigenvalue = eigenvalues(i);
                maxEigenValueIndex = i;
            }
        }

        maxEigenvector = eigenSolver.eigenvectors().col(maxEigenValueIndex);
        q = Eigen::Quaterniond(maxEigenvector(0), maxEigenvector(1), maxEigenvector(2), maxEigenvector(3));
        rotMat = q.toRotationMatrix();

        R = Mat::zeros(3, 3, CV_64F);
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                R.at<double>(i, j) = rotMat(i, j);
            }
        }

        T = meanCam2 - (R * meanCam1);

        success = true;
    }

    return success;
}

bool IntrinsicExtrinsicCalib::findCornersMono(Size boardSize, Size subPixSize, vector<Mat> &images,
                                           vector<vector<Point2f> > &cornersCam)
{
    bool foundSuccess = true;
    vector<Point2f> corners;
    bool patternFound;
    Mat viewGray;
    int i;
    int imageCount = 0;
    vector<Mat> newImageOrder;

    #pragma omp parallel default (shared) private(i, patternFound, corners, viewGray) num_threads(omp_get_max_threads())
    #pragma omp for schedule(static)
    for (i = 0; i < images.size(); ++i)
    {
        patternFound = findChessboardCorners(images[i], boardSize, corners,
                                             CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);

        if (patternFound)
        {
            vector<Point2f> subPixCorners(corners);

            cvtColor(images[i], viewGray, CV_BGR2GRAY);

            cornerSubPix(viewGray, subPixCorners, subPixSize,
                         Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

            #pragma omp critical
            {
                cornersCam.push_back(subPixCorners);
                newImageOrder.push_back(images[i]);
            }
        }

        #pragma omp critical
        {
            imageCount++;
            cout << "[findCornersMono] progress: " << ((int)(((double)imageCount / images.size()) * 100)) << "%" << endl;
        }
    }

    images.clear();
    images = newImageOrder;

    if (cornersCam.size() == 0)
    {
        printError("findCornersMono", "No pattern found!");
        foundSuccess = false;
    }

    return foundSuccess;
}

double IntrinsicExtrinsicCalib::computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                                      const vector<vector<Point2f> >& imagePoints,
                                                      const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                                      const Mat& cameraMatrix , const Mat& distCoeffs,
                                                      vector<float>& perViewErrors)
{
    vector<Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for( i = 0; i < (int)objectPoints.size(); ++i )
    {
        projectPoints( Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,
                       distCoeffs, imagePoints2);
        err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);

        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float) std::sqrt(err*err/n);
        totalErr        += err*err;
        totalPoints     += n;
    }

    return std::sqrt(totalErr/totalPoints);
}

bool IntrinsicExtrinsicCalib::calculateIntrinsic(Size boardSize, Size imageSize, Mat &cameraMatrix, Mat &distCoeffs,
                                            vector<vector<Point2f> > corners, vector<Mat>& rvecs, vector<Mat>& tvecs,
                                            vector<float>& reprojErrs,  double& totalAvgErr, int flags)
{
    bool ok;
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    vector<Point3f> boardPoints;
    vector<vector<Point3f> > objectPoints;

    cameraMatrix.at<double>(0,0) = 1.0;
    distCoeffs = Mat::zeros(8, 1, CV_64F);

    calcBoardCornerPositions(boardSize, 5, boardPoints);

    for (int i = 0; i < corners.size(); i++)
        objectPoints.push_back(boardPoints);

    double rms = calibrateCamera(objectPoints, corners, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flags);

    ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

    totalAvgErr = computeReprojectionErrors(objectPoints, corners,
                                            rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

    return ok;
}

vector<int> IntrinsicExtrinsicCalib::findBestCalibParams(ParamCombination &paramCombi, Size boardSize,Size imageSize,
                                                         Mat &cameraMatrix, Mat &distCoeffs, vector<vector<Point2f> > imagePoints,
                                                         bool createEvalFile, string savePath)
{
    int flag;
    vector<double> errors;
    int lowestErrorPos;
    double lowestError = numeric_limits<double>::max();
    vector<Mat> cameraMatTemp;
    vector<Mat> distCoeffsTemp;
    int i;
    int caliCount = 0;
    vector<vector<int> > params = paramCombi.createParamCombinations();

    vector<float> reprojErrs;
    double totalAvgErr;
    vector<Mat> rVecsTemp;
    vector<Mat> tVecsTemp;

    cameraMatTemp.resize(params.size());
    distCoeffsTemp.resize(params.size());
    errors.resize(params.size());

    // Berechne alle Kalibriermoeglichkeiten mit entstandenem Error
    #pragma omp parallel default (shared) private(i, reprojErrs, totalAvgErr, rVecsTemp, tVecsTemp, flag) num_threads(omp_get_max_threads())
    #pragma omp for schedule(static)
    for (i = 0; i < params.size(); i++)
    {
        flag = paramCombi.getFlag(params.at(i));

        calculateIntrinsic(boardSize, imageSize, cameraMatTemp.at(i), distCoeffsTemp.at(i), imagePoints, rVecsTemp,
                                                        tVecsTemp, reprojErrs, totalAvgErr, flag);

        rVecsTemp.clear();
        tVecsTemp.clear();

        errors.at(i) = totalAvgErr;
        totalAvgErr = 0;

        #pragma omp critical
        {
            caliCount++;
            cout << "[findBestCalibParams] progress: " << ((int)(((double)caliCount / params.size()) * 100)) << "%" << endl;
        }
    }

    for (i = 0; i < errors.size(); i++)
    {
        if (errors.at(i) < lowestError)
        {
            lowestError = errors.at(i);
            lowestErrorPos = i;
        }
    }

    cameraMatrix = cameraMatTemp.at(lowestErrorPos);
    distCoeffs = distCoeffsTemp.at(lowestErrorPos);

    if (createEvalFile)
    {
        savePath += "eval";
        paramCombi.createParamEval(params, errors, savePath);
    }

    return params.at(lowestErrorPos);
}
