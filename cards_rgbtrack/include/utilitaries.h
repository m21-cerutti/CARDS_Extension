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

extern struct RectStruct;
extern struct Target;
extern struct Color32;
extern struct Frame;

// Transforme texture frame in Mat OpenCV
Mat TextureToCVMat( const Frame& frame );

//Convert Rect2d to Rect
RectStruct Rect2dToRectStruct( const Rect2d& rect );

//Convert Rect to Rect2d
Rect2d Rect2dToRectStruct( const RectStruct& rect );

// Debug an image with waitkey
void DebugMat( const Mat& frame );

// Debug targets represented by rect2d
void DebugCVTargets( const Mat& mat,const std::vector<Rect2d>& objects,int number );
void DebugCVTargets( const Mat& mat,const Target* targets,int number );

// Create tracker by name
Ptr<Tracker> createTrackerByName( const std::string& name );

#endif // !UTLITARIES_HPP

