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
						Gregory Kramida     (algomorph@gmail.com)
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

#include "blockmatching.h"
#include <iostream>

#if defined(CV_VERSION_EPOCH) || (CV_VERSION_MAJOR < 3)
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
#else
//==================== OPENCV 3 ====================================================================
BlockMatching::BlockMatching()
{
    this->bm = cv::StereoBM::create(64, 21);
    this->bm->setDisp12MaxDiff(1);
}

void BlockMatching::SetPreFilterCap(int value)
{
    this->bm->setPreFilterCap(value);
}

void BlockMatching::SetPreFilterSize(int value)
{
    this->bm->setPreFilterSize(value);
}

void BlockMatching::SetMinDisparity(int value)
{
    this->bm->setMinDisparity(value);
}

void BlockMatching::SetSADWindowSize(int value)
{
    this->bm->setBlockSize(value);
}

void BlockMatching::SetNumberOfDisparities(int value)
{
    this->bm->setNumDisparities(value);
}

void BlockMatching::SetTextureThreshold(int value)
{
    this->bm->setTextureThreshold(value);
}

void BlockMatching::SetUniquenessRatio(int value)
{
    this->bm->setUniquenessRatio(value);
}

void BlockMatching::SetSpeckleWindowSize(int value)
{
    this->bm->setSpeckleWindowSize(value);
}

void BlockMatching::SetSpeckleRange(int value)
{
    this->bm->setSpeckleRange(value);
}

#endif


void BlockMatching::setImages(cv::Mat &imageLeft, cv::Mat &imageRight)
{
    this->imageLeft = imageLeft.clone();
    this->imageRight = imageRight.clone();
}

Mat BlockMatching::updateBM()
{
    Mat res, res8;

#if defined(CV_VERSION_EPOCH) || (CV_VERSION_MAJOR < 3)
    this->bm(this->imageLeft, this->imageRight, res);
#else // opencv3
    this->bm->compute(this->imageLeft, this->imageRight, res);
#endif
    res.convertTo(res8, CV_8U, 1 / 16.);

    return res8;
}
