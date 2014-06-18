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

#include "adcensuscv.h"

ADCensusCV::ADCensusCV(const Mat &leftImage, const Mat &rightImage, Size censusWin, float lambdaAD, float lambdaCensus)
{
    this->leftImage = leftImage;
    this->rightImage = rightImage;
    this->censusWin = censusWin;
    this->lambdaAD = lambdaAD;
    this->lambdaCensus = lambdaCensus;
}

float ADCensusCV::ad(int wL, int hL, int wR, int hR) const
{
    float dist = 0;
    const Vec3b &colorLP = leftImage.at<Vec3b>(hL, wL);
    const Vec3b &colorRP = rightImage.at<Vec3b>(hR, wR);

    for(uchar color = 0; color < 3; ++color)
    {
        dist += std::abs(colorLP[color] - colorRP[color]);
    }
    return (dist / 3);
}

float ADCensusCV::census(int wL, int hL, int wR, int hR) const
{
    float dist = 0;
    const Vec3b &colorRefL = leftImage.at<Vec3b>(hL, wL);
    const Vec3b &colorRefR = rightImage.at<Vec3b>(hR, wR);

    for(int h = -censusWin.height / 2; h <= censusWin.height / 2; ++h)
    {
        for(int w = -censusWin.width / 2; w <= censusWin.width / 2; ++w)
        {
            const Vec3b &colorLP = leftImage.at<Vec3b>(hL + h, wL + w);
            const Vec3b &colorRP = rightImage.at<Vec3b>(hR + h, wR + w);
            for(uchar color = 0; color < 3; ++color)
            {
                // bool diff = (colorLP[color] < colorRefL[color]) ^ (colorRP[color] < colorRefR[color]);
                bool diff = (colorLP[color] - colorRefL[color]) * (colorRP[color] - colorRefR[color]) < 0;
                dist += (diff)? 1: 0;
            }
        }
    }

    return dist;
}

float ADCensusCV::adCensus(int wL, int hL, int wR, int hR) const
{
    float dist;

    // compute Absolute Difference cost
    float cAD = ad(wL, hL, wR, hR);

    // compute Census cost
    float cCensus = census(wL, hL, wR, hR);

    // combine the two costs
    dist = 1 - exp(-cAD / lambdaAD);
    dist += 1 - exp(-cCensus / lambdaCensus);

    return dist;
}
