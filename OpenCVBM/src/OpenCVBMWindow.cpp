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

#include "OpenCVBMWindow.h"

#include <iostream>
#include <sstream>
#include <QFileDialog>
#include <QMessageBox>

using namespace cv;
using namespace std;

OpenCVBMWindow::OpenCVBMWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    this->ui.setupUi(this);

    this->bmi = new BlockmatchingImage();

    this->autoUpdateEnabled = this->ui.cb_auto_update->isChecked();

    this->setup();
}

OpenCVBMWindow::~OpenCVBMWindow()
{
    delete this->bmi;
}

void OpenCVBMWindow::slot_slider_PreFilterCap(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_PreFilterCap->setText(valueString.str().c_str());
    this->bm.SetPreFilterCap(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_slider_PreFilterSize(int value)
{
    this->on_Slider_PreFilterSize_sliderReleased();
}

void OpenCVBMWindow::slot_slider_MinDisparity(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_MinDisparity->setText(valueString.str().c_str());
    this->bm.SetMinDisparity(value);

    if (this->autoUpdateEnabled)
        this->update();
}
void OpenCVBMWindow::slot_slider_SADWindowSize(int value)
{
    this->on_Slider_SADWindowSize_sliderReleased();
}

void OpenCVBMWindow::slot_slider_NumberOfDisparities(int value)
{
    this->on_Slider_NumberOfDisparities_sliderReleased();
}

void OpenCVBMWindow::slot_slider_TextureThreshold(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_TextureThreshold->setText(valueString.str().c_str());
    this->bm.SetTextureThreshold(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_slider_UniquenessRatio(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_UniquenessRatio->setText(valueString.str().c_str());
    this->bm.SetUniquenessRatio(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_slider_SpeckleWindowSize(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_SpeckleWindowSize->setText(valueString.str().c_str());
    this->bm.SetSpeckleWindowSize(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_slider_SpeckleRange(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_SpeckleRange->setText(valueString.str().c_str());
    this->bm.SetSpeckleRange(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::setImages(cv::Mat &imageLeft, cv::Mat &imageRight)
{
    Mat il, rl;

    il = imageLeft.clone();
    rl = imageRight.clone();

    this->bm.setImages(il, rl);
    this->sgbm.setImages(il, rl);

    this->bmi->setWindowSize(il.cols, rl.rows);

    this->update();
}

void OpenCVBMWindow::setup()
{
    stringstream value;    

    bmi->show();

    this->ui.pb_update->setEnabled(!this->autoUpdateEnabled);

    value << this->ui.Slider_MinDisparity->value();
    this->ui.label_MinDisparity->setText(value.str().c_str());
    this->bm.SetMinDisparity(this->ui.Slider_MinDisparity->value());
    value.str("");

    value << this->ui.Slider_NumberOfDisparities->value();
    this->ui.label_NumberOfDisparities->setText(value.str().c_str());
    this->bm.SetNumberOfDisparities(this->ui.Slider_NumberOfDisparities->value());
    value.str("");

    value << this->ui.Slider_PreFilterCap->value();
    this->ui.label_PreFilterCap->setText(value.str().c_str());
    this->bm.SetPreFilterCap(this->ui.Slider_PreFilterCap->value());
    value.str("");

    value << this->ui.Slider_PreFilterSize->value();
    this->ui.label_PreFilterSize->setText(value.str().c_str());
    this->bm.SetPreFilterSize(this->ui.Slider_PreFilterSize->value());
    value.str("");

    value << this->ui.Slider_SADWindowSize->value();
    this->ui.label_SADWindowSize->setText(value.str().c_str());
    this->bm.SetSADWindowSize(this->ui.Slider_SADWindowSize->value());
    value.str("");

    value << this->ui.Slider_SpeckleRange->value();
    this->ui.label_SpeckleRange->setText(value.str().c_str());
    this->bm.SetSpeckleRange(this->ui.Slider_SpeckleRange->value());
    value.str("");

    value << this->ui.Slider_SpeckleWindowSize->value();
    this->ui.label_SpeckleWindowSize->setText(value.str().c_str());
    this->bm.SetSpeckleWindowSize(this->ui.Slider_SpeckleWindowSize->value());
    value.str("");

    value << this->ui.Slider_TextureThreshold->value();
    this->ui.label_TextureThreshold->setText(value.str().c_str());
    this->bm.SetTextureThreshold(this->ui.Slider_TextureThreshold->value());
    value.str("");

    value << this->ui.Slider_UniquenessRatio->value();
    this->ui.label_UniquenessRatio->setText(value.str().c_str());
    this->bm.SetUniquenessRatio(this->ui.Slider_UniquenessRatio->value());
    value.str("");

    value << this->ui.Slider_MinDisparitySG->value();
    this->ui.label_MinDisparitySG->setText(value.str().c_str());
    this->sgbm.SetMinDisparity(this->ui.Slider_MinDisparitySG->value());
    value.str("");

    value << this->ui.Slider_NumberOfDisparitiesSG->value();
    this->ui.label_NumberOfDisparitiesSG->setText(value.str().c_str());
    this->sgbm.SetNumberOfDisparities(this->ui.Slider_NumberOfDisparitiesSG->value());
    value.str("");

    value << this->ui.Slider_UniquenessRatioSG->value();
    this->ui.label_UniquenessRatioSG->setText(value.str().c_str());
    this->sgbm.SetUniquenessRatio(this->ui.Slider_UniquenessRatioSG->value());
    value.str("");

    value << this->ui.Slider_SpeckleWindowSizeSG->value();
    this->ui.label_SpeckleWindowSizeSG->setText(value.str().c_str());
    this->sgbm.SetSpeckleWindowSize(this->ui.Slider_SpeckleWindowSizeSG->value());
    value.str("");

    value << this->ui.Slider_SpeckleRangeSG->value();
    this->ui.label_SpeckleRangeSG->setText(value.str().c_str());
    this->sgbm.SetSpeckleRange(this->ui.Slider_SpeckleRangeSG->value());
    value.str("");

    value << this->ui.Slider_SADWindowSizeSG->value();
    this->ui.label_SADWindowSizeSG->setText(value.str().c_str());
    this->sgbm.SetSADWindowSize(this->ui.Slider_SADWindowSizeSG->value());
    value.str("");
}

void OpenCVBMWindow::setScale(double scale)
{
    this->bmi->setScale(scale);
}

void OpenCVBMWindow::update()
{
    if (!this->bmi->isVisible())
        this->bmi->show();

    if (this->ui.tb_bm->currentIndex() == 0) //First Tab
        this->actImg = this->bm.updateBM();
    else
        this->actImg = this->sgbm.updateSGBM();

    this->bmi->updateImage(this->actImg);
}

void OpenCVBMWindow::slot_reset()
{
    bool autoUpChecked = this->ui.cb_auto_update->isChecked();

    if (autoUpChecked)
        this->ui.cb_auto_update->setChecked(false);

    if (this->ui.tb_bm->currentIndex() == 0)
    {
        this->slot_slider_PreFilterCap(31);
        this->ui.Slider_PreFilterCap->setValue(31);
        this->slot_slider_PreFilterSize(5);
        this->ui.Slider_PreFilterSize->setValue(5);
        this->slot_slider_SADWindowSize(9);
        this->ui.Slider_SADWindowSize->setValue(9);
        this->slot_slider_MinDisparity(0);
        this->ui.Slider_MinDisparity->setValue(0);
        this->slot_slider_NumberOfDisparities(48);
        this->ui.Slider_NumberOfDisparities->setValue(48);
        this->slot_slider_TextureThreshold(10);
        this->ui.Slider_TextureThreshold->setValue(10);
        this->slot_slider_UniquenessRatio(15);
        this->ui.Slider_UniquenessRatio->setValue(15);
        this->slot_slider_SpeckleWindowSize(100);
        this->ui.Slider_SpeckleWindowSize->setValue(100);
        this->slot_slider_SpeckleRange(32);
        this->ui.Slider_SpeckleRange->setValue(32);
    }
    else
    {
        this->slot_slider_MinDisparitySG(0);
        this->ui.Slider_MinDisparitySG->setValue(0);
        this->slot_slider_NumberOfDisparitiesSG(48);
        this->ui.Slider_NumberOfDisparitiesSG->setValue(48);
        this->slot_slider_UniquenessRatioSG(15);
        this->ui.Slider_UniquenessRatioSG->setValue(15);
        this->slot_slider_SpeckleWindowSizeSG(100);
        this->ui.Slider_SpeckleWindowSizeSG->setValue(100);
        this->slot_slider_SpeckleRangeSG(32);
        this->ui.Slider_SpeckleRangeSG->setValue(32);
        this->slot_slider_SADWindowSizeSG(9);
        this->ui.Slider_SADWindowSizeSG->setValue(9);
    }

    if (autoUpChecked)
        this->ui.cb_auto_update->setChecked(true);
}


void OpenCVBMWindow::on_Slider_NumberOfDisparities_sliderReleased()
{
    stringstream valueString;
    int value = this->ui.Slider_NumberOfDisparities->value();
    int raster = value % 16;

    if (raster > (value / 2))
        raster = (value % 16) - raster;
    else
        raster = raster * -1;

    value = value + raster;

    this->ui.Slider_NumberOfDisparities->setSliderPosition(value);

    valueString << value;
    this->ui.label_NumberOfDisparities->setText(valueString.str().c_str());
    this->bm.SetNumberOfDisparities(value);

    if (((value % 16) == 0) && this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::on_Slider_PreFilterSize_sliderReleased()
{
    stringstream valueString;
    int value = this->ui.Slider_PreFilterSize->value();

    if (!(value % 2))
        value -= 1;

    this->ui.Slider_PreFilterSize->setSliderPosition(value);

    valueString << value;
    this->ui.label_PreFilterSize->setText(valueString.str().c_str());
    this->bm.SetPreFilterSize(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::on_Slider_SADWindowSize_sliderReleased()
{

    stringstream valueString;
    int value = this->ui.Slider_SADWindowSize->value();

    if (!(value % 2))
        value -= 1;

    this->ui.Slider_SADWindowSize->setSliderPosition(value);

    valueString << value;
    this->ui.label_SADWindowSize->setText(valueString.str().c_str());
    this->bm.SetSADWindowSize(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_slider_MinDisparitySG(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_MinDisparitySG->setText(valueString.str().c_str());
    this->sgbm.SetMinDisparity(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_slider_SADWindowSizeSG(int value)
{
    this->on_Slider_SADWindowSizeSG_sliderReleased();
}

void OpenCVBMWindow::slot_slider_NumberOfDisparitiesSG(int value)
{
    this->on_Slider_NumberOfDisparitiesSG_sliderReleased();
}

void OpenCVBMWindow::slot_slider_UniquenessRatioSG(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_UniquenessRatioSG->setText(valueString.str().c_str());
    this->sgbm.SetUniquenessRatio(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_slider_SpeckleWindowSizeSG(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_SpeckleWindowSizeSG->setText(valueString.str().c_str());
    this->sgbm.SetSpeckleWindowSize(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_slider_SpeckleRangeSG(int value)
{
    stringstream valueString;

    valueString << value;
    this->ui.label_SpeckleRangeSG->setText(valueString.str().c_str());
    this->sgbm.SetSpeckleRange(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_update()
{
    this->update();
}

void OpenCVBMWindow::slot_auto_update(int state)
{
    if (this->ui.cb_auto_update->isChecked())
    {
        this->autoUpdateEnabled = true;
        this->ui.pb_update->setDisabled(true);
    }
    else
    {
        this->autoUpdateEnabled = false;
        this->ui.pb_update->setEnabled(true);
    }
}

void OpenCVBMWindow::on_Slider_NumberOfDisparitiesSG_sliderReleased()
{
    stringstream valueString;
    int value = this->ui.Slider_NumberOfDisparitiesSG->value();
    int raster = value % 16;

    if (raster > (value / 2))
        raster = (value % 16) - raster;
    else
        raster = raster * -1;

    value = value + raster;

    this->ui.Slider_NumberOfDisparitiesSG->setSliderPosition(value);

    valueString << value;
    this->ui.label_NumberOfDisparitiesSG->setText(valueString.str().c_str());
    this->sgbm.SetNumberOfDisparities(value);

    if (((value % 16) == 0) && this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::on_Slider_SADWindowSizeSG_sliderReleased()
{
    stringstream valueString;
    int value = this->ui.Slider_SADWindowSizeSG->value();

    if (!(value % 2))
        value -= 1;

    this->ui.Slider_SADWindowSizeSG->setSliderPosition(value);

    valueString << value;
    this->ui.label_SADWindowSizeSG->setText(valueString.str().c_str());
    this->sgbm.SetSADWindowSize(value);

    if (this->autoUpdateEnabled)
        this->update();
}

void OpenCVBMWindow::slot_bmtab_changed(int value)
{
    this->update();
}

void OpenCVBMWindow::slot_save_image()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "",
                                tr("Images (*.png *.bmp *.jpg)"));

    if (fileName.length() != 0)
    {
        imwrite(fileName.toStdString(), this->actImg);
    }
}

void OpenCVBMWindow::slot_save_disparity()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "",
                                tr("Disparities (*.xml)"));

    if (fileName.length() != 0)
    {
        FileStorage fS(fileName.toStdString(), FileStorage::WRITE);
        fS << "disp" << this->actImg;
        fS.release();
    }
}

void OpenCVBMWindow::slot_quit()
{
    this->bmi->close();
    this->close();
}

void OpenCVBMWindow::setSavePath(std::string path)
{
    this->savePath = path;
}
