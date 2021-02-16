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
enum class State_Tracker {
	Undefined = -1,
	Live,
	Occluded,
	Lost
};

struct Target
{
	Target(short id) : ID(id), pos(0), size(0), state() {}
	short ID;
	Vec2i pos;
	Vec2i size;
	State_Tracker state;
};

/*Use for frame pass from C# to C++*/
struct Color32
{
	uchar red;
	uchar green;
	uchar blue;
	uchar alpha;
};

struct Frame
{
	Color32** rawData;
	int width;
	int height;
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
