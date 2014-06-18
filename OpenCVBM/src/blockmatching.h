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

#ifndef BLOCKMATCHING_H
#define BLOCKMATCHING_H

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

class BlockMatching
{

public:

    BlockMatching();

    void SetPreFilterCap(int value);

    void SetPreFilterSize(int value);

    void SetMinDisparity(int value);

    void SetSADWindowSize(int value);

    void SetNumberOfDisparities(int value);

    void SetTextureThreshold(int value);

    void SetUniquenessRatio(int value);

    void SetSpeckleWindowSize(int value);

    void SetSpeckleRange(int value);

    void setImages(cv::Mat &imageLeft, cv::Mat &imageRight);

    Mat updateBM(void);

private:

    StereoBM bm;

    cv::Mat imageLeft;

    cv::Mat imageRight;
};

#endif // BLOCKMATCHING_H
