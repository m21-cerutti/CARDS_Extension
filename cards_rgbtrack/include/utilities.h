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

extern struct Matrix3x3f;
extern struct RectStruct;
extern struct Target;
extern struct Frame;

/// @brief Convert texture in rawdata frame to Mat OpenCV
/// @param frame Yhe frame to convert.
/// @return The resulting Mat in BGR format.
Mat FrameToCVMat( const Frame& frame );

/// @brief Fill the rawdata field with mat texture.
/// @param src The source.
/// @param dst The destination. Other fields than rawData are not touched, verify before the matrix correspond to frame.
void CVMatToFrameRawData( const Mat& src,Frame& dst );

/// @brief Copy data of a frame.
/// @param src The source.
/// @return The copied frame.
void CopyFrame( const Frame& src,Frame& dst );

/// @brief Convert Rect2d to Rect
/// @param rect Source
/// @return A RectStruct.
RectStruct Rect2dToRectStruct( const Rect2d& rect );

/// @brief Convert Rect to Rect2d
/// @param rect Source
/// @return A Rect2d
Rect2d RectStructToRect2d( const RectStruct& rect );

/// @brief Convert Matrix3x3f to Mat
/// @param rect Source
/// @return A Mat OpenCV
Mat Matrix3x3fToMat( const Matrix3x3f& mat );

/// @brief Convert Mat to Matrix3x3f
/// @param mat Source
/// @return A Matrix3x3f
Matrix3x3f MatToMatrix3x3f( const Mat& mat );

/// @brief Debug an image with waitkey.
/// @param Frame 
void DebugMat( const Mat& frame,const string win_name = "DebugOpenCV" );

/// @brief Debug in OpenCV window the actual frame and targets. Green if Live, Red otherwise.
/// @param frame The current frame with objects.
/// @param targets An allocated array of targets.
/// @param maxTarget The number maximum of targets.
void DebugCVTargets( const Mat& mat,const Target* targets,const int maxTarget );

/// @brief Create tracker by name.
/// @param name 
/// Refer to existing tracker in OpenCV without GOTURN that need deeplearning network datas.
/// COLOR for a custom colorTracker.
/// @return The pointer of a  tracker allocated.
Ptr<ITracker> createTrackerByName( const std::string& name );


