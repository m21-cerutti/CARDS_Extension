#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


/*Tacker definition*/
enum class StateTracker {
	Undefined = -1,
	Live,
	Occluded,
	Lost
};

struct Target
{
	Target() : ID(-1), object(), state() {}
	short ID;
	Rect2d object;
	StateTracker state;
};

/*Use for frame pass from C# to C++*/
struct Color32
{
	Color32() : red(0), green(0), blue(0), alpha(0) {}
	uchar red;
	uchar green;
	uchar blue;
	uchar alpha;
};

struct Frame
{
	Frame() : rawData(), width(0), height(0) {}
	int width;
	int height;
	Color32* rawData;
};

/**
 * @brief
 * @return
*/
extern "C"  int __declspec(dllexport) __stdcall Init();

/**
 * @brief
 * @return
*/
extern "C"  void __declspec(dllexport) __stdcall Close();

/**
 * @brief
 * @param rawImage
 * @param targets
 * @param nbTarget
 * @param maxTarget
 * @return
*/
extern "C"  void __declspec(dllexport) __stdcall Detect(Frame & frame, Target * targets, int& nbTarget, int maxTarget);

/**
 * @brief
 * @param rawImage
 * @param targets
 * @param nbTarget
 * @return
*/
extern "C"  void __declspec(dllexport) __stdcall Track(Frame & frame, Target * targets, int nbTarget);
