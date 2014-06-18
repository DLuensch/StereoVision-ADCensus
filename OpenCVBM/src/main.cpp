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

#include "OpenCVBMWindow.h"
#include <QApplication>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cv::Mat imageLeft, imageRight, img8Left, img8Right;
    double scale;

    OpenCVBMWindow w;

    if (argc >= 3)
    {
        imageLeft = cv::imread((string)argv[1], CV_LOAD_IMAGE_GRAYSCALE);
        imageRight = cv::imread((string)argv[2], CV_LOAD_IMAGE_GRAYSCALE);


//        imshow("left", img8Left);
//        imshow("right", img8Right);

//        waitKey();

        if (argc > 3)
            scale = atof(argv[3]);
        else
            scale = 0.5;

        if (imageLeft.data && imageRight.data)
        {
            w.setScale(scale);
            w.setImages(imageLeft, imageRight);
            w.setSavePath((string)argv[1]);
            w.show();

            return a.exec();
        }
        else
        {
            if (!imageLeft.data)
                cout << "Konnte linkes Bild nicht oeffnen!" << endl;
            if (!imageRight.data)
                cout << "Konnte rechtes Bild nicht oeffnen!" << endl;
        }
    }
    else if ((argc == 2) && ((strcmp(argv[1], "help") == 0) || (strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--h") == 0)))
    {
        std::cout << "Aufruf: image_left_cam   image_right_cam   scale_factor_window" << endl;
    }
    else
    {
        std::cout << "Fehler beim starten des Programmes! Aufruf: image_left_cam   image_right_cam   scale_factor_window" << endl;
    }

    return 0;
}
