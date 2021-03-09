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
	float x;      //in px
	float y;      //in px
	float width;  //in px
	float height; //in px
};

/*Tacker definition*/
enum class StateTracker
{
	Undefined = 0,
	Live,
	Occluded,
	Lost
};

struct Target
{
	Target() : ID( -1 ),rect(),state( StateTracker::Undefined )
	{
	}
	short ID;
	RectStruct rect;
	//TODO angle or rotation matrix
	StateTracker state;
};

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
	Frame() : rawData(),width( 0 ),height( 0 ),is_flipped( false )
	{
	}
	int width;
	int height;
	bool is_flipped;
	Color32* rawData;
};


extern "C"
{
	// Init the tracking context
	EXPORT_API void __stdcall Init( Target* targets,int& nbTarget,const int maxTarget );

	// Close the tracking context
	EXPORT_API void __stdcall Close( Target* targets,int& nbTarget,const int maxTarget );

	// Register an object for tracking
	EXPORT_API void __stdcall Register( const Frame& frame,const RectStruct& zoneObject,Target* targets,int& nbTarget,const int maxTarget );

	// Unregister object
	EXPORT_API void __stdcall UnRegister( const int id,Target* targets,int& nbTarget );

	// Detect on a zone if a new object was detected and register it.
	EXPORT_API void __stdcall Detect( const Frame& frame,const RectStruct& zoneDetection,Target* targets,int& nbTarget,const int maxTarget );

	// Check the tracking accuracy, occlusion with detection or heavy operations and correct it.
	EXPORT_API void __stdcall CheckTrack( const Frame& frame,Target* targets,const int nbTarget );

	// Update the taget object with light tracking
	EXPORT_API void __stdcall Track( const Frame& frame,Target* targets,const int nbTarget );
}
