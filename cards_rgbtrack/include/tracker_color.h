#pragma once

#include "interface_tracker.h"

class TrackerCOLOR : public ITracker
{
private:
	Mat bgr_frame;
	Mat hsv_frame;
	Mat thresh_frame;

	short delta = 10;
	short color = 0;

	int minArea = 10;

	short findColor( Mat img );

public:
	TrackerCOLOR();
	bool init( InputArray image,const Rect2d& boundingBox ) override;
	bool update( InputArray image,Rect2d& boundingBox ) override;
	static Ptr<ITracker> create();
};