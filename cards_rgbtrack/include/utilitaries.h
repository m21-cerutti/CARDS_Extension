#pragma once
#ifndef UTLITARIES_HPP
#define UTLITARIES_HPP

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

extern struct Color32;
extern struct Frame;

/**
 * @brief
 * @param texData
 * @param width
 * @param height
*/
Mat TextureToCVMat(const Frame& frame);

/**
 * @brief
 * @param frame
*/
void DebugMat(const Mat& frame);


#endif // !UTLITARIES_HPP

