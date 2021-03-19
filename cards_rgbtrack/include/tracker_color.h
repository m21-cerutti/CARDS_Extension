#pragma once

#include "interface_tracker.h"

//TODO DOC
class TrackerCOLOR : public ITracker
{
public:

	TrackerCOLOR();

	bool init( InputArray image,const Rect2d& boundingBox ) override;

	bool update( InputArray image,Rect2d& boundingBox ) override;

	static Ptr<ITracker> create();

private:

	Mat bgr_frame;
	Mat hsv_frame;
	Mat thresh_frame;

	short color = 0;
	//TODO #define
	short epsilon = 10;
	int minArea = 10;

	short findColor( Mat img );
};