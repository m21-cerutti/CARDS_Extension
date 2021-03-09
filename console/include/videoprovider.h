#pragma once

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/utility.hpp>

#include "tracker_api.h"

using namespace std;
using namespace cv;

class VideoProvider
{
public:
	//Camera
	VideoProvider();

	//VideoFile
	VideoProvider( string filename );

	~VideoProvider();

	virtual const Frame& GetFrame();

private:
	void InitCameraFrame( int width,int height );

	VideoCapture _cap;
	Frame _frame;
};