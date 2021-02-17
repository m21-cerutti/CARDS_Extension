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

// Transforme texture frame in Mat OpenCV
Mat TextureToCVMat(Frame& frame);

// Debug an image with waitkey
void DebugMat(const Mat& frame);

// Debug targets represented by rect2d
void DebugTargets(const Mat& mat, const std::vector<Rect2d>& objects, int number);

// Create tracker by name
Ptr<Tracker> createTrackerByName(const std::string& name);


#endif // !UTLITARIES_HPP

