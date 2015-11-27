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

#if defined(CV_VERSION_EPOCH) || (CV_VERSION_MAJOR < 3)
    StereoSGBM sgbm;
#else
    cv::Ptr<StereoSGBM> sgbm;
#endif
    cv::Mat imageLeft;

    cv::Mat imageRight;

    void calcP(void);
};

#endif // BLOCKMATCHING_H
