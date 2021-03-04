#pragma once

#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/*Structure data*/

struct Vector3f
{
	float x;
	float y;
	float z;
};

struct Matrix4x4
{
	float c_00;
	float c_01;
	float c_02;
	float c_03;
	float c_10;
	float c_11;
	float c_12;
	float c_13;
	float c_20;
	float c_21;
	float c_22;
	float c_23;
	float c_30;
	float c_31;
	float c_32;
	float c_33;
};

struct RectStruct
{
	float x; //in cm
	float y; //in cm
	float width;
	float height;
};

/*Tacker definition*/
enum class StateTracker
{
	Undefined = -1,
	Live,
	Occluded,
	Lost
};

struct Target
{
	Target() : ID( -1 ),rect(),state()
	{
	}
	short ID;
	RectStruct rect;
	StateTracker state;
};

//TODO Structure export ?

/*Use for frame pass from C# to C++*/
struct Color32
{
	Color32() : red( 0 ),green( 0 ),blue( 0 ),alpha( 0 )
	{
	}
	uchar red;
	uchar green;
	uchar blue;
	uchar alpha;
};

struct Frame
{
	Frame() : rawData(),width( 0 ),height( 0 )
	{
	}
	int width;
	int height;
	Color32* rawData;
};


extern "C"
{
#pragma region Plugin Methods

	EXPORT_API void __stdcall DebugTargets( const Frame& frame,Target* targets,const int nbTarget );

#pragma endregion

#pragma region Plugin Methods

	EXPORT_API int __stdcall Init();

	EXPORT_API void __stdcall Close();

	EXPORT_API void __stdcall Detect( const Frame& frame,Target* targets,int& nbTarget,const int maxTarget );

	EXPORT_API void __stdcall Track( const Frame& frame,Target* targets,const int nbTarget );

#pragma endregion
}
