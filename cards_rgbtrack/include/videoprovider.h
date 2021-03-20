#pragma once

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/utility.hpp>

#include "tracker_api.h"

using namespace std;
using namespace cv;

struct VideoProvider
{
public:
	/// @brief Open the camera with VideoCapture and initialise frame memory.
	/// @param width The requested width.
	/// @param height The requested height.
	VideoProvider( int width,int height );

	/// @brief Open a video with VideoCapture and initialise frame memory.
	/// @param filename The path of the video.
	/// @param width The requested width.
	/// @param height The requested height.
	VideoProvider( string filename,int width,int height );

	~VideoProvider();

	/// @brief Get a frame from the media open.
	/// @return The reference to intern frame, with field rawdata set to nullptr if not suceed.
	virtual const Frame& GetFrame();
	virtual const Frame& GetCopyFrame();

private:
	/// @brief Init the intern frame and videocapture options.
	/// @param width The requested width.
	/// @param height The requested height.
	void InitCameraFrame( int width,int height );

private:
	VideoCapture _cap;
	Frame _frame;
};