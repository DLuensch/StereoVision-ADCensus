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

#include "blockmatching.h"
#include <iostream>

BlockMatching::BlockMatching()
{
    this->bm = StereoBM(StereoBM::BASIC_PRESET, 64, 21);
    this->bm.state->disp12MaxDiff = 1;
}

void BlockMatching::SetPreFilterCap(int value)
{
    this->bm.state->preFilterCap = value;
}

void BlockMatching::SetPreFilterSize(int value)
{
    this->bm.state->preFilterSize = value;
}

void BlockMatching::SetMinDisparity(int value)
{
    this->bm.state->minDisparity = value;
}

void BlockMatching::SetSADWindowSize(int value)
{
    this->bm.state->SADWindowSize = value;
}

void BlockMatching::SetNumberOfDisparities(int value)
{
    this->bm.state->numberOfDisparities = value;
}

void BlockMatching::SetTextureThreshold(int value)
{
    this->bm.state->textureThreshold = value;
}

void BlockMatching::SetUniquenessRatio(int value)
{
    this->bm.state->uniquenessRatio = value;
}

void BlockMatching::SetSpeckleWindowSize(int value)
{
    this->bm.state->speckleWindowSize = value;
}

void BlockMatching::SetSpeckleRange(int value)
{
    this->bm.state->speckleRange = value;
}

void BlockMatching::setImages(cv::Mat &imageLeft, cv::Mat &imageRight)
{
    this->imageLeft = imageLeft.clone();
    this->imageRight = imageRight.clone();
}

Mat BlockMatching::updateBM()
{
    Mat res, res8;

    this->bm(this->imageLeft, this->imageRight, res);

    res.convertTo(res8, CV_8U, 1 / 16.);

    return res8;
}
