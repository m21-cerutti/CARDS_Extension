#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <stdio.h>

#include "interface_tracker.h"

using namespace std;
using namespace cv;

extern struct RectStruct;
extern struct Target;
extern struct Frame;

// Convert texture in rawdata frame to Mat OpenCV
Mat FrameToCVMat( const Frame& frame );

// Fill the rawdata field with mat texture
void CVMatToFrameRawData( const Mat& src,Frame& dst );

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
Ptr<ITracker> createTrackerByName( const std::string& name );


