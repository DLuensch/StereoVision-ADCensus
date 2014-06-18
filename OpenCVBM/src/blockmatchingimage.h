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

#ifndef BLOCKMATCHINGIMAGE_H
#define BLOCKMATCHINGIMAGE_H

#include "ui_blockmatchingimage.h"

#include <opencv2/core/core.hpp>
#include <QDialog>

class BlockmatchingImage : public QDialog
{
    Q_OBJECT

public:
    BlockmatchingImage(QWidget *parent = 0);

    ~BlockmatchingImage();

    void updateImage(cv::Mat img);

    void setScale(double value);

    void setWindowSize(int x, int y);

private:

    Ui::BlockmatchingImage ui;

    double scale;
};

#endif // BLOCKMATCHINGIMAGE_H
