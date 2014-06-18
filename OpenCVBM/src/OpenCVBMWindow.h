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

    void setSavePath(string path);

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
