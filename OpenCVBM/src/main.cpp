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
