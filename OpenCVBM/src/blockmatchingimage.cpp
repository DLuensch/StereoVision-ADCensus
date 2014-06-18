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

#include "blockmatchingimage.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

BlockmatchingImage::BlockmatchingImage(QWidget *parent)
{
    this->ui.setupUi(this);

    this->scale = 1.0;
}

BlockmatchingImage::~BlockmatchingImage() { }

void BlockmatchingImage::updateImage(cv::Mat img)
{
    if (this->scale != 1.0)
    {
        cv::resize(img, img, cv::Size(), scale, scale);
    }

    cv::Mat colorImg;
    cv::cvtColor(img, colorImg, CV_GRAY2RGB);
    QPixmap image = QPixmap::fromImage(QImage((unsigned char*) colorImg.data, colorImg.cols, colorImg.rows, colorImg.step, QImage::Format_RGB888));

    this->ui.image_field->setPixmap(image);
}

void BlockmatchingImage::setScale(double value)
{
    this->scale = value;
}

void BlockmatchingImage::setWindowSize(int x, int y)
{
    this->resize(((int)(x * this->scale)), ((int)(y * this->scale)));
}
