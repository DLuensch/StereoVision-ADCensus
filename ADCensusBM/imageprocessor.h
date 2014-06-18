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

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <opencv2/opencv.hpp>
#include "common.h"

using namespace cv;

class ImageProcessor
{
public:
    ImageProcessor(float percentageOfDeletion);
    Mat stretchHistogram(Mat image);
    Mat unsharpMasking(Mat image, string blurMethod, int kernelSize, float alpha, float beta);
    Mat laplacianSharpening(Mat image, int kernelSize, float alpha, float beta);
private:
    float percentageOfDeletion;
};

#endif // IMAGEPROCESSOR_H
