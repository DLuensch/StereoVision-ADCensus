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

#ifndef ADCENSUSCV_H
#define ADCENSUSCV_H

#include <opencv2/opencv.hpp>

using namespace cv;

class ADCensusCV
{
public:
    ADCensusCV(const Mat &leftImage, const Mat &rightImage, Size censusWin, float lambdaAD, float lambdaCensus);
    float ad(int wL, int hL, int wR, int hR) const;
    float census(int wL, int hL, int wR, int hR) const;
    float adCensus(int wL, int hL, int wR, int hR) const;
private:
    Mat leftImage;
    Mat rightImage;
    Size censusWin;
    float lambdaAD;
    float lambdaCensus;
};

#endif // ADCENSUSCV_H
