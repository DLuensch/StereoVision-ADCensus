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

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

void addText(Mat &img, float disp, float shiftY)
{
    stringstream text;

    text.str("");
    text << "disp.: " << disp * -1<< "  ||  shiftY: " << shiftY;

    putText(img, text.str(), Point(img.rows * 0.05, img.cols * 0.05), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255));
}

Mat overlayImg(Mat img1, Mat img2, double alphaImg1, double alphaImg2)
{
    Mat dst;

    addWeighted(img1, alphaImg1, img2, alphaImg2, 0.0, dst);

    return dst;
}

Mat warp(Mat leftImg, Mat rightImg, int shiftX, int shiftY, double alphaLeft, double alphaRight)
{
    Mat dst = Mat::zeros(leftImg.cols, leftImg.rows, leftImg.type());
    Mat tmpLeftImg = leftImg.clone(), tmpRightImg = rightImg.clone();

    Mat m = Mat::zeros(3,3, CV_32FC1);

    if (shiftX > 0)
    {
        m.at<int>(0,2) = shiftX;
        m.at<int>(1,2) = 0;
        m.at<int>(0,0) = 1; m.at<int>(1,1) = 1; m.at<int>(2,2) = 1;

        warpPerspective(leftImg, tmpLeftImg, m, leftImg.size());
    }
    else if (shiftX < 0)
    {
        m.at<int>(0,2) = abs(shiftX);
        m.at<int>(1,2) = 0;
        m.at<int>(0,0) = 1; m.at<int>(1,1) = 1; m.at<int>(2,2) = 1;

        warpPerspective(rightImg, tmpRightImg, m, rightImg.size());
    }

    if (shiftY > 0)
    {
        m.at<int>(0,2) = 0;
        m.at<int>(1,2) = shiftY;
        m.at<int>(0,0) = 1; m.at<int>(1,1) = 1; m.at<int>(2,2) = 1;

        warpPerspective(tmpLeftImg, tmpLeftImg, m, leftImg.size());
    }
    else if (shiftY < 0)
    {
        m.at<int>(0,2) = 0;
        m.at<int>(1,2) = abs(shiftY);
        m.at<int>(0,0) = 1; m.at<int>(1,1) = 1; m.at<int>(2,2) = 1;

        warpPerspective(tmpRightImg, tmpRightImg, m, rightImg.size());
    }

    dst = overlayImg(tmpLeftImg, tmpRightImg, alphaLeft, alphaRight);

    return dst;
}

int main(int argc, char** argv)
{
    Mat leftImg, rightImg, showImg;
    double alphaLeft, alphaRight;
    int shiftX = 0, shiftY = 0;
    bool noError = false;
    char key;
    double scaleFactor;

    if (argc == 6)
    {
        leftImg = imread(argv[1]);
        rightImg = imread(argv[2]);
        alphaLeft = atof(argv[3]);
        alphaRight = atof(argv[4]);
        scaleFactor = atof(argv[5]);
        noError = true;

        resize(leftImg, leftImg, Size(), scaleFactor, scaleFactor);
        resize(rightImg, rightImg, Size(), scaleFactor, scaleFactor);
    }
    else if ((argc >= 2) && ((strcmp(argv[1], "help") == 0) || (strcmp(argv[1], "-h") == 0)|| (strcmp(argv[1], "--h") == 0)))
    {
        cout << "Parameter: leftIMG-Path, rightIMG-Path, leftIMG-Alpha, rightIMG-Alpha, Img-View-Scale" << endl;
    }
    else
    {
        cout << "Nicht alle / falsche Parameter angegeben!" << endl;
    }

    if (noError)
    {
        showImg = overlayImg(leftImg, rightImg, alphaLeft, alphaRight);
        addText(showImg, shiftX / scaleFactor, shiftY / scaleFactor);
        imshow("PictureOverlay", showImg);

        do
        {
            key = waitKey();

            switch (key)
            {
                case 'a':
                    shiftX++;
                    break;
                case 'd':
                    shiftX--;
                    break;
                case 'w':
                    shiftY--;
                    break;
                case 's':
                    shiftY++;
                    break;
            }

            showImg = warp(leftImg, rightImg, shiftX, shiftY, alphaLeft, alphaRight);
            addText(showImg, shiftX / scaleFactor, shiftY / scaleFactor);
            imshow("PictureOverlay", showImg);

        } while (key != 'q' && key != 27);

        imwrite("dst.jpg", overlayImg(leftImg, rightImg, alphaLeft, alphaRight));
    }

    return 0;
}
