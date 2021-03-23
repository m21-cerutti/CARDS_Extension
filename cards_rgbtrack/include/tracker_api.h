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
* We use for the inputs the OpenCV Convention for RectStruct that represent an object in screen space in pixel.
* The coordinate (0,0) is at top-left corner at the image. Watchout for compatibility with Unity that use the bottom-left corner
* as origin.
* No extra work are needed to convert images in this convention, since FlipMode in Frame structure permet to correct it for specific devices conventions,
* by default None is the conversion from Unity to OpenCV standart.
*/

#pragma region Basic structures

struct Vector2f
{
	float x = 0;
	float y = 0;
};

struct Vector3f
{
	float x = 0;
	float y = 0;
	float z = 0;
};

struct Matrix3x3f
{
	float c_00 = 0;
	float c_01 = 0;
	float c_02 = 0;
	float c_10 = 0;
	float c_11 = 0;
	float c_12 = 0;
	float c_20 = 0;
	float c_21 = 0;
	float c_22 = 0;
};

struct Matrix4x4f
{
	float c_00 = 0;
	float c_01 = 0;
	float c_02 = 0;
	float c_03 = 0;
	float c_10 = 0;
	float c_11 = 0;
	float c_12 = 0;
	float c_13 = 0;
	float c_20 = 0;
	float c_21 = 0;
	float c_22 = 0;
	float c_23 = 0;
	float c_30 = 0;
	float c_31 = 0;
	float c_32 = 0;
	float c_33 = 0;
};

struct RectStruct
{
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;
};


#pragma endregion

#pragma region Target tracker structures

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
	Target() : id( -1 ),rect(),original_size(),state( StateTracker::Undefined )
	{
	}
	short id;
	RectStruct rect;
	Vector2f original_size;
	StateTracker state;
};

struct PoseParameters
{
	PoseParameters() : intrinsic_camera(),dist_cam( 0 ),meter_pixel_x_ratio( 0 ),meter_pixel_y_ratio( 0 )
	{
	}
	Matrix3x3f intrinsic_camera;
	float dist_cam;
	float meter_pixel_x_ratio;
	float meter_pixel_y_ratio;
};

#pragma endregion

#pragma region Frame structures

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

#pragma endregion

/* Plugin */

extern "C"
{

	/// @brief Init the tracking context and internal memory.
	/// @param targets The array of targets, need to be initialised in memory.
	/// @param nbTarget The number of targets tyhat will be set to zero.
	/// @param maxTarget The maximum number of targets this context will handle.
	EXPORT_API void __stdcall Init( Target* targets,int& nbTarget,const int maxTarget );

	/// @brief Close the tracking context and free internal datas.
	/// @param targets The array of targets
	/// @param nbTarget The number of targets
	/// @param maxTarget The maximum number of targets
	EXPORT_API void __stdcall Close( Target* targets,int& nbTarget,const int maxTarget );

	/// @brief Register a new target to tracking context.
	/// @param frame The current frame. 
	/// @param zone The zone of the object bounding box, watchout see convention for more information.
	/// @param targets The array of targets
	/// @param nbTarget The number of targets
	/// @param maxTarget The maximum number of targets
	EXPORT_API int __stdcall Register( const Frame& frame,const RectStruct& zoneObject,Target* targets,int& nbTarget,const int maxTarget );

	/// @brief Unregister a given target from tracking context.
	/// @param id The id of the object
	/// @param targets The array of targets
	/// @param nbTarget The number of targets
	EXPORT_API void __stdcall UnRegister( const int id,Target* targets,int& nbTarget );

	/// @brief Permit to detect if a new object is inserted to the zoneDetection, and register it.
	/// Watchout to already registered objects, unregister before.
	/// @param frame The current frame
	/// @param zoneDetection The zone of area of detection, watchout see convention for more information.
	/// @param targets The array of targets
	/// @param nbTarget The number of targets
	/// @param maxTarget The maximum number of targets
	EXPORT_API void __stdcall Detect( const Frame& frame,const Frame& frameBackground,const RectStruct& zoneDetection,Target* targets,int& nbTarget,const int maxTarget );

	/// @brief Check if the Track algorithm have made mistakes, and correct it.
	/// Also permit to research object lost, occluded or out of camera to actualise their state.
	/// Use it only periodically and not in each frame.
	/// @param frame The current frame
	/// @param targets The array of targets
	/// @param maxTarget The maximum number of targets
	EXPORT_API void __stdcall CheckTrack( const Frame& frame,Target* targets,const int maxTarget );

	/// @brief Track the objects registered and give an approximation of their location.
	/// Mistakes can be made, and some cases are not handle for performances issues.
	/// Can be use at each frame.
	/// @param frame The current frame
	/// @param targets The array of targets
	/// @param maxTarget The maximum number of targets
	EXPORT_API void __stdcall Track( const Frame& frame,Target* targets,const int maxTarget );

	/// @brief Estimate the pose of the object. 
	/// @param target The target to estimate.
	/// @param params The parameters needed for pose.
	/// @return An homegenous matrix, with rotation and position, X and Y are screen cordiantes, Z is a distance ratio relative to table.
	EXPORT_API Matrix4x4f __stdcall EstimatePose( const Target& target,const PoseParameters& params );
}
