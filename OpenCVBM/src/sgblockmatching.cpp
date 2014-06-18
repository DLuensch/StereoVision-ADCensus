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

#include "sgblockmatching.h"

SGBlockMatching::SGBlockMatching()
{
    this->sgbm.disp12MaxDiff = 1;
    this->sgbm.fullDP = false;
    this->sgbm.preFilterCap = 63;
    this->sgbm.SADWindowSize = 9;
}

void SGBlockMatching::SetMinDisparity(int value)
{
    this->sgbm.minDisparity = value;
}

void SGBlockMatching::SetSADWindowSize(int value)
{
    this->sgbm.SADWindowSize = value;

    this->calcP();
}

void SGBlockMatching::SetNumberOfDisparities(int value)
{
    this->sgbm.numberOfDisparities = value;
}

void SGBlockMatching::SetUniquenessRatio(int value)
{
    this->sgbm.uniquenessRatio = value;
}

void SGBlockMatching::SetSpeckleWindowSize(int value)
{
    this->sgbm.speckleWindowSize = value;
}

void SGBlockMatching::SetSpeckleRange(int value)
{
    this->sgbm.speckleRange = value;
}

void SGBlockMatching::setImages(cv::Mat &imageLeft, cv::Mat &imageRight)
{
    this->imageLeft = imageLeft.clone();
    this->imageRight = imageRight.clone();

    this->calcP();
}

void SGBlockMatching::calcP()
{
    this->sgbm.P1 = 8 * imageLeft.channels() * this->sgbm.SADWindowSize * this->sgbm.SADWindowSize;
    this->sgbm.P1 = 32 * imageLeft.channels() * this->sgbm.SADWindowSize * this->sgbm.SADWindowSize;
}

Mat SGBlockMatching::updateSGBM()
{
    Mat res, res8;

    this->sgbm(this->imageLeft, this->imageRight, res);

    res.convertTo(res8, CV_8U, 1 / 16.);

    return res8;
}
