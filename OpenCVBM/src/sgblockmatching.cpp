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
						Gregory Kramida     (algomorph@gmail.com
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

#include "sgblockmatching.h"

#if defined(CV_VERSION_EPOCH) || (CV_VERSION_MAJOR < 3)

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

void SGBlockMatching::calcP()
{
    this->sgbm.P1 = 8 * imageLeft.channels() * this->sgbm.SADWindowSize * this->sgbm.SADWindowSize;
    this->sgbm.P1 = 32 * imageLeft.channels() * this->sgbm.SADWindowSize * this->sgbm.SADWindowSize;
}
#else
SGBlockMatching::SGBlockMatching():sgbm(cv::StereoSGBM::create(0,256,9,0,0,1,63))
{
    this->sgbm->setMode(cv::StereoSGBM::MODE_SGBM);
}

void SGBlockMatching::SetMinDisparity(int value)
{
    this->sgbm->setMinDisparity(value);
}

void SGBlockMatching::SetSADWindowSize(int value)
{
    this->sgbm->setBlockSize(value);

    this->calcP();
}

void SGBlockMatching::SetNumberOfDisparities(int value)
{
    this->sgbm->setNumDisparities(value);
}

void SGBlockMatching::SetUniquenessRatio(int value)
{
    this->sgbm->setUniquenessRatio(value);
}

void SGBlockMatching::SetSpeckleWindowSize(int value)
{
    this->sgbm->setSpeckleWindowSize(value);
}

void SGBlockMatching::SetSpeckleRange(int value)
{
    this->sgbm->setSpeckleRange(value);
}

void SGBlockMatching::calcP()
{
    this->sgbm->setP1(8 * imageLeft.channels() * this->sgbm->getBlockSize() * this->sgbm->getBlockSize());
    this->sgbm->setP2(32 * imageLeft.channels() * this->sgbm->getBlockSize() * this->sgbm->getBlockSize());
}
#endif

void SGBlockMatching::setImages(cv::Mat &imageLeft, cv::Mat &imageRight)
{
    this->imageLeft = imageLeft.clone();
    this->imageRight = imageRight.clone();

    this->calcP();
}

Mat SGBlockMatching::updateSGBM()
{
    Mat res, res8;

    this->sgbm->compute(this->imageLeft, this->imageRight, res);

    res.convertTo(res8, CV_8U, 1 / 16.);

    return res8;
}
