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

#ifndef INTRINSICEXTRINSICCALIB_H
#define INTRINSICEXTRINSICCALIB_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <libconfig.h++>

#ifdef WIN32
# include <Eigen/Dense>
#else
# include <eigen3/Eigen/Dense>
#endif

#include "paramcombination.h"

using namespace std;
using namespace cv;

namespace IntrinsicExtrinsicCalib
{

    /**
     * @brief printError
     * @param errorSource
     * @param errorMsg
     */
    void printError(string errorSource, string errorMsg);


    bool readStereoParams(string confPath, Size &boardSize, float &squareSize, string &xmlImages,
                          Size &subPixSize, string &savePath,
                          string &ymlIntrinsic1, string &ymlIntrinsic2);


    bool readMonoParams(string confPath, Size &boardSize, string &xmlImages,
                        Size &subPixSize, string &savePath, bool &autoMode, int &flags);

    /**
     * @brief loadStereoIntrinsics
     * @param ymlIntrinsic1
     * @param ymlIntrinsic2
     * @param cameraMatrix1
     * @param cameraMatrix2
     * @param distCoeffs1
     * @param distCoeffs2
     */
    void loadStereoIntrinsics(string &ymlIntrinsic1, string &ymlIntrinsic2,
                              Mat &cameraMatrix1, Mat &cameraMatrix2,
                              Mat &distCoeffs1,  Mat &distCoeffs2);

    /**
     * @brief loadImageList
     * @param file
     * @param list
     * @return
     */
    bool loadImageList(string file, vector<string> &list);

    /**
     * @brief loadImages
     * @param fileList
     * @param images
     * @return
     */
    bool loadImages(vector<string> fileList, vector<Mat> &images);

    /**
     * @brief undistortStereoImages
     * @param inputImages
     * @param outputImages
     * @param cameraMatrix1
     * @param cameraMatrix2
     * @param distCoeffs1
     * @param distCoeffs2
     * @return
     */
    bool undistortStereoImages(vector<Mat> &inputImages, vector<Mat> &outputImages,
                               Mat &cameraMatrix1, Mat &cameraMatrix2,
                               Mat &distCoeffs1,  Mat &distCoeffs2);

    /**
     * @brief findCornersStereo
     * @param boardSize
     * @param subPixSize
     * @param images
     * @param cornersCam1
     * @param cornersCam2
     * @return
     */
    bool findCornersStereo(Size boardSize, Size subPixSize, vector<Mat> &images,
                           vector<vector<Point2f> > &cornersCam1,
                           vector<vector<Point2f> > &cornersCam2);

    /**
     * @brief calcBoardCornerPositions
     * @param boardSize
     * @param squareSize
     * @param corners
     */
    void calcBoardCornerPositions(Size boardSize, float squareSize, vector<Point3f>& corners);

    /**
     * @brief calcMean
     * @param points
     * @param mean
     */
    void calcMean(vector<Mat> &points, Mat &mean);

    /**
     * @brief calculateRT
     * @param tvecs1
     * @param tvecs2
     * @param R
     * @param T
     * @return
     */
    bool calculateRT(vector<Mat> &tvecs1, vector<Mat> &tvecs2, Mat &R, Mat &T);

    /**
     * @brief findCornersMono
     * @param boardSize
     * @param subPixSize
     * @param images
     * @param cornersCam
     * @return
     */
    bool findCornersMono(Size boardSize, Size subPixSize, vector<Mat> &images,
                         vector<vector<Point2f> > &cornersCam);

    /**
     * @brief computeReprojectionErrors
     * @param objectPoints
     * @param imagePoints
     * @param rvecs
     * @param tvecs
     * @param cameraMatrix
     * @param distCoeffs
     * @param perViewErrors
     * @return
     */
    double computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                                      const vector<vector<Point2f> >& imagePoints,
                                      const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                                      const Mat& cameraMatrix , const Mat& distCoeffs,
                                      vector<float>& perViewErrors);


    bool calculateIntrinsic(Size boardSize, Size imageSize, Mat &cameraMatrix, Mat &distCoeffs,
                            vector<vector<Point2f> > corners, vector<Mat>& rvecs, vector<Mat>& tvecs,
                            vector<float> &reprojErrs, double &totalAvgErr, int flags);

    vector<int> findBestCalibParams(ParamCombination &paramCombi, Size boardSize,Size imageSize,
                                    Mat &cameraMatrix, Mat &distCoeffs, vector<vector<Point2f> > imagePoints,
                                    bool createEvalFile = false, string savePath = "");
}

#endif // INTRINSICEXTRINSICCALIB_H


