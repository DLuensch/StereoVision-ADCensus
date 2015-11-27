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

#ifndef OpenCVBMWindow_H
#define OpenCVBMWindow_H

#include "blockmatchingimage.h"
#include "blockmatching.h"
#include "sgblockmatching.h"
#include "ui_OpenCVBMWindow.h"

#include <QtGui/QMainWindow>
#include <opencv2/opencv.hpp>

class OpenCVBMWindow : public QMainWindow
{
	Q_OBJECT

public Q_SLOTS:

    void slot_slider_PreFilterCap(int value);

    void slot_slider_PreFilterSize(int value);

    void slot_slider_MinDisparity(int value);

    void slot_slider_MinDisparitySG(int value);

    void slot_slider_SADWindowSize(int value);

    void slot_slider_SADWindowSizeSG(int value);

    void slot_slider_NumberOfDisparities(int value);

    void slot_slider_NumberOfDisparitiesSG(int value);

    void slot_slider_TextureThreshold(int value);

    void slot_slider_UniquenessRatio(int value);

    void slot_slider_UniquenessRatioSG(int value);

    void slot_slider_SpeckleWindowSize(int value);

    void slot_slider_SpeckleWindowSizeSG(int value);

    void slot_slider_SpeckleRange(int value);

    void slot_slider_SpeckleRangeSG(int value);

    void slot_update(void);

    void slot_reset(void);

    void slot_auto_update(int state);

    void slot_bmtab_changed(int value);

    void slot_save_image();

    void slot_save_disparity();

    void slot_quit();

public:

	OpenCVBMWindow(QWidget *parent = 0, Qt::WFlags flags = 0);

	~OpenCVBMWindow();

    void setImages(cv::Mat &imageLeft, cv::Mat &imageRight);

    void setScale(double scale);

    void setSavePath(std::string path);

private slots:

    void on_Slider_NumberOfDisparities_sliderReleased();

    void on_Slider_PreFilterSize_sliderReleased();

    void on_Slider_SADWindowSize_sliderReleased();

    void on_Slider_NumberOfDisparitiesSG_sliderReleased();

    void on_Slider_SADWindowSizeSG_sliderReleased();

private:

    Ui::OpenCVBM ui;

    BlockmatchingImage* bmi;

    BlockMatching bm;

    SGBlockMatching sgbm;

    Mat actImg;

    bool autoUpdateEnabled;

    void setup(void);

    void update(void);

    String savePath;
};

#endif // OpenCVBMWindow_H
