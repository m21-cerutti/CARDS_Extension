#pragma once

#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

#include <opencv2/core.hpp>

#include <iostream>

using namespace std;
using namespace cv;

/*
* CONVENTIONS :
*
*/

/*Structure data*/

struct Vector3f
{
	float x;
	float y;
	float z;
};

struct Matrix4x4f
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

/* Tracker definition */

enum class StateTracker
{
	Lost = -1,
	Undefined = 0,
	Live,
	Occluded,
	OutOfCamera
};

struct Target
{
	Target() : id( -1 ),rect(),state( StateTracker::Undefined )
	{
	}
	short id;
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

enum class FlipMode
{
	None = 0,
	FlipBoth,
	FlipY,
	FlipX,
};

struct Frame
{
	Frame() : rawData(),width( 0 ),height( 0 ),flip_mode( FlipMode::None )
	{
	}
	int width;
	int height;
	FlipMode flip_mode;
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
	// TODO
	EXPORT_API void __stdcall Detect( const Frame& frame,const RectStruct& zoneDetection,Target* targets,int& nbTarget,const int maxTarget );

	// Check the tracking accuracy, occlusion with detection or heavy operations and correct it.
	//TODO
	EXPORT_API void __stdcall CheckTrack( const Frame& frame,Target* targets,const int nbTarget );

	// Update the taget object with light tracking
	EXPORT_API void __stdcall Track( const Frame& frame,Target* targets,const int nbTarget );

	// TODO
	EXPORT_API void __stdcall EstimatePose( const Frame& frame,Target* targets,const int nbTarget );
}
