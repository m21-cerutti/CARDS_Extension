#pragma once

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/utility.hpp>

#include "tracker_api.h"

using namespace std;
using namespace cv;

class VideoProviderConsole
{
public:
	//Camera
	VideoProviderConsole();

	//VideoFile
	VideoProviderConsole( string filename );

	~VideoProviderConsole();

	virtual const Frame& GetFrame();
	virtual const Frame& CopyFrame();

private:
	void InitCameraFrame( int width,int height );

	VideoCapture _cap;
	Frame _frame;
	Frame _frameBackground;
};