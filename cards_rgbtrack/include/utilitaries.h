#pragma once
#ifndef UTLITARIES_HPP
#define UTLITARIES_HPP

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>

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
Mat TextureToCVMat(Frame& frame);

/**
 * @brief
 * @param frame
*/
void DebugMat(const Mat& frame);

/**
 * @brief
 * @param mat
 * @param objects
*/
void DebugTargets(const Mat& mat, const std::vector<Rect2d>& objects, int number);

/**
 * @brief
 * @param name
 * @return
*/
Ptr<Tracker> createTrackerByName(const std::string& name);


#endif // !UTLITARIES_HPP

