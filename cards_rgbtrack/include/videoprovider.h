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
	//Camera
	VideoProvider( int width,int height );

	//VideoFile
	VideoProvider( string filename,int width,int height );

	~VideoProvider();

	virtual const Frame& GetFrame();
	virtual const Frame& GetCopyFrame();

private:
	void InitCameraFrame( int width,int height );

	VideoCapture _cap;
	Frame _frame;
};