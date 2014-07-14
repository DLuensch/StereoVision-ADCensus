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

#include "paramcombination.h"
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace cv;

ParamCombination::ParamCombination()
{
    this->allCombinations = 7; // 2^3 - 1

    this->paramEval[0] = 1;
    this->paramEval[1] = 2;
    this->paramEval[2] = 4;
    this->paramEval[3] = 8;
    this->paramEval[4] = 16;

    this->opencvParam[0] = CV_CALIB_FIX_PRINCIPAL_POINT;
    this->opencvParam[1] = CV_CALIB_FIX_ASPECT_RATIO;
    this->opencvParam[2] = CV_CALIB_ZERO_TANGENT_DIST;
    this->opencvParam[3] = CV_CALIB_RATIONAL_MODEL;
    this->opencvParam[4] = CV_CALIB_FIX_K1;
    this->opencvParam[5] = CV_CALIB_FIX_K2;
    this->opencvParam[6] = CV_CALIB_FIX_K3;
    this->opencvParam[7] = CV_CALIB_FIX_K4;
    this->opencvParam[8] = CV_CALIB_FIX_K5;
    this->opencvParam[9] = CV_CALIB_FIX_K6;

    this->opencvParamNames[0] = "CV_CALIB_FIX_PRINCIPAL_POINT (FPP)";
    this->opencvParamNames[1] = "CV_CALIB_FIX_ASPECT_RATIO (FAR)";
    this->opencvParamNames[2] = "CV_CALIB_ZERO_TANGENT_DIST (ZTD)";
    this->opencvParamNames[3] = "CV_CALIB_RATIONAL_MODEL (RM)";
    this->opencvParamNames[4] = "CV_CALIB_FIX_K1 (FK1)";
    this->opencvParamNames[5] = "CV_CALIB_FIX_K2 (FK2)";
    this->opencvParamNames[6] = "CV_CALIB_FIX_K3 (FK3)";
    this->opencvParamNames[7] = "CV_CALIB_FIX_K4 (FK4)";
    this->opencvParamNames[8] = "CV_CALIB_FIX_K5 (FK5)";
    this->opencvParamNames[9] = "CV_CALIB_FIX_K6 (FK6)";
}

void ParamCombination::createConfig(string imagesXml, string savePath,
                                    int hessboardWidth, int chessboardHeight,
                                    int subPixSize, vector<int> paramCombi)
{
    stringstream stream;
    string fileName = savePath;
    fileName += "config.cfg";
    ofstream ostream(fileName.c_str(), ofstream::binary);

    if (ostream.is_open())
    {
        stream << "imagesXml = " << "\"" << imagesXml << "\""   << ";" << endl;
        stream << "savePath = " << "\"" << savePath << "\""   << ";" << endl;
        stream << "chessboardWidth = " << hessboardWidth << ";" << endl;
        stream << "chessboardHeight = " << chessboardHeight << ";" << endl;
        stream << "subPixSize = " << subPixSize << ";" << endl;
        stream << "autoCalibrate = " << "FALSE" << ";" << endl;
        stream << "FIX_PRINCIPAL_POINT = " << ((paramCombi.at(0) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "FIX_ASPECT_RATIO = " << ((paramCombi.at(1) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "ZERO_TANGENT_DIST = " << ((paramCombi.at(2) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "RATIONAL_MODEL = " << ((paramCombi.at(3) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "FIX_K1 = " << ((paramCombi.at(4) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "FIX_K2 = " << ((paramCombi.at(5) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "FIX_K3 = " << ((paramCombi.at(6) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "FIX_K4 = " << ((paramCombi.at(7) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "FIX_K5 = " << ((paramCombi.at(8) > 0) ? "TRUE" : "FALSE") << ";" << endl;
        stream << "FIX_K6 = " << ((paramCombi.at(9) > 0) ? "TRUE" : "FALSE") << ";" << endl;

        ostream << stream.str().c_str();
        ostream.close();
    }

}

int ParamCombination::getFlag(vector<int> paramCombi)
{
    int flag = 0;

    for (int i = 0; i < paramCombi.size(); i++)
    {
        flag += paramCombi.at(i);
    }

    return flag;
}

void ParamCombination::createParamEval(vector<vector<int> > paramCombinations, vector<double> errors, string fileName)
{
    stringstream csv;

    if (fileName.find(".csv") == String::npos)
        fileName += ".csv";

    ofstream ostream(fileName.c_str(), ofstream::binary);

    // erstelle kategorien
    csv << "NR;";
    for (int i = 0; i < 10; i++)
    {
        csv << this->opencvParamNames[i] << ";";
    }
    csv << "ERROR VALUE" << endl;

    for (int i = 0; i < paramCombinations.size(); i++)
    {
        csv << (i + 1) << ";";
        for (int j = 0; j < paramCombinations.at(i).size(); j++)
        {
            csv << ((paramCombinations.at(i).at(j) > 0) ? 1 : 0) << ";";
        }
        csv << errors.at(i) << endl;
    }

    if (ostream.is_open())
    {
        ostream << csv.str();

        ostream.close();
    }
}

string ParamCombination::getParamString(vector<int> paramCombi)
{
    stringstream res;

    res.str("");

    for (int i = 0; i < paramCombi.size(); i++)
    {
        if (paramCombi.at(i) != 0)
            res << this->opencvParamNames[i] << (i < (paramCombi.size() - 1) ? " " : "");
    }

    return res.str();
}

vector<vector<int> > ParamCombination::createParamCombinations()
{
    vector<vector<int> > paramCombinations;

    // Erstelle alle Parameterkonfigurationen fuer alle K Parameter
    for (int i = 0, fixK = 0; i <= 6; i++, fixK++)
    {
        for (int j = 0, combination = 1; j < allCombinations; j++, combination++)
        {
            vector<int> temp;

            for (int k = 0; k < 3; k++)
            {
                temp.push_back(((combination & this->paramEval[k]) > 0) ? this->opencvParam[k] : 0);
            }

            // Setze Rational Model erst ab K4 - K6
            temp.push_back(((i > 3) ? this->opencvParam[3] : 0));

            for (int k = 0; k < 6; k++)
            {
                if (k >= fixK)
                    temp.push_back(this->opencvParam[k + 4]);
                else
                    temp.push_back(0);
            }

            paramCombinations.push_back(temp);

        }
    }

    // nur fuer testausgaben
//    for (int i = 0, nr = 1; i < allCombinations * 7; i++, nr++)
//    {
//        cout << nr << (nr < 10 ? " : " : ": ");

//        for (int j = 0; j < paramCombinations.at(0).size(); j++)
//        {
//            cout << ((paramCombinations.at(i).at(j) == this->opencvParam[j]) ? 1 : 0) << " ";
//        }

//        cout << endl;
//    }

    return paramCombinations;
}
