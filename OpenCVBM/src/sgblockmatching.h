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

#ifndef SGBLOCKMATCHING_H
#define SGBLOCKMATCHING_H

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

class SGBlockMatching
{

public:

    SGBlockMatching();

    void SetMinDisparity(int value);

    void SetNumberOfDisparities(int value);

    void SetUniquenessRatio(int value);

    void SetSpeckleWindowSize(int value);

    void SetSpeckleRange(int value);

    void SetSADWindowSize(int value);

    void setImages(cv::Mat &imageLeft, cv::Mat &imageRight);

    Mat updateSGBM(void);

private:

    StereoSGBM sgbm;

    cv::Mat imageLeft;

    cv::Mat imageRight;

    void calcP(void);
};

#endif // BLOCKMATCHING_H
