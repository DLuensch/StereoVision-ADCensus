StereoVision-ADCensus
=====================
This is a master student project realized at the robotics laboratory of the Westphalian University of Applied Science.
With the programs in this repository you can calibrate single cameras, a stereo system, calculate disparity maps and evaluate your data. It's a all in one package for stereo vision! The software runs under windows and linux and is written in C++.

Based on the following libraries:
* OpenCV
* BOOST
* libConfig
* PCL

Projects in this package:
* IntrinsicExtrinsicCalib
* ImageRectify 
* ADCensusBM 
* PictureOverlay 
* OpenCVBM 
* evalDisp 

# IntrinsicExtrinsicCalib
With this program you can calculate the intrinsic and extrinsinc parameters of your cameras. The calibration needs pictures with chessboard patterns in any size you want. In the project we used a 18x12 (only the inner edges count) pattern on a A0 paper. Every calibration is based on about 20 Images from different views, distances and angles. It is important that the pattern is visible on every section of the camera sensor! You have to take these images for every camera and calibration step (intrinsic and extrinsic).

### Building and execution on a linux based system

**Build the program on:**
 1. Navigate into the "IntrinsicExtrinsicCalib" folder
 2. `mkdir build`
 3. `cd build`
 4. `cmake ..`
 5. `make`
 6. If you can build the program you should see 2 files called "intrisic" and "extrinsic"

**Execute the intrinsic:**
 1.  Create a "intrinsicConfig.cfg" as you can see in the sample folder. (e.g.: configIntrinsicLeft.cfg, configIntrinsicRight.cfg)
 2.  Create a "intrinsicImages.xml" as you can see in the sample folder. (e.g.: imagesLeftIntrinsic.xml, imagesRightIntrinsic.xml)
 3.  Execute the intrisic calibration: `./intrinsic PATH_TO_YOUR_CONFIG/intrinsicConfig.cfg`
 4.  This can take several minutes. Control your calibration result in your destination folder!
  
**Execute the extrinsic:** 
 1.  Create a "extrinsicConfig.cfg" as you can see in the sample folder. (e.g.: configExtrinsic.cfg)
 2.  Create a "extrinsicImages.xml" as you can see in the sample folder. (e.g.: imagesExtrinsic.xml)
 3.  Execute the extrinsic calibration: `./extrinsic PATH_TO_YOUR_CONFIG/extrinsicConfig.cfg`
 4.  This can take several minutes. Control your calibration result in your destination folder!
 5.  Control the translation vector in the resulting 4x4 matrix. This should match the distance between the cameras in the selected measuring unit.

# ImageRectify 
What does the program do and how I use it? Comming soon!

# ADCensusBM 
What does the program do and how I use it? Comming soon!

# PictureOverlay 
What does the program do and how I use it? Comming soon!

# OpenCVBM 
What does the program do and how I use it? Comming soon!

# evalDisp 
What does the program do and how I use it? Comming soon!
