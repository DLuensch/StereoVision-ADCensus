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

#ifndef PARAMCOMBINATION_H
#define PARAMCOMBINATION_H

#include <vector>
#include <string>

#define FPP 0 // CV_CALIB_FIX_PRINCIPAL_POINT
#define FAR 1 // CV_CALIB_FIX_ASPECT_RATIO
#define ZTD 2 // CV_CALIB_ZERO_TANGENT_DIST
#define RM  3 // CV_CALIB_RATIONAL_MODEL
#define FK1  4 // Fix KX ...
#define FK2  5
#define FK3  6
#define FK4  7
#define FK5  8
#define FK6  9

using namespace std;

class ParamCombination
{

public:
    ParamCombination();

    vector<vector<int> > createParamCombinations();

    string getParamString(vector<int> paramCombi);

    void createParamEval(vector<vector<int> > paramCombinations, vector<double> errors, string fileName);

    int getFlag(vector<int> paramCombi);

    void createConfig(string imagesXml, string savePath, int hessboardWidth,
                      int chessboardHeight,int subPixSize, vector<int> paramCombi);

private:
    int allCombinations;
    int paramEval[4];
    int opencvParam[10];
    string opencvParamNames[10];

};

#endif // PARAMCOMBINATION_H
