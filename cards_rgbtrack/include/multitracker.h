#pragma once

#include  <unordered_map>
#include "opencv2/tracking/tracker.hpp"

#include "interface_tracker.h"

using namespace std;
using namespace cv;

/// Based on Multi-Tracker Classes ---By Tyan Vladimir---
class MultiTrackerCARDS
{
public:
	MultiTrackerCARDS();

	~MultiTrackerCARDS();

	void add( const int id,InputArray image,const Rect2d& boundingBox,Ptr<ITracker> tracker_algorithm );

	void remove( const int id );

	bool update( const int id,InputArray image );

	void correct( const int id,InputArray image,Rect2d boundingBoxe );

	Rect2d getBoundinBox( const int id );

	Scalar getColor( const int id );

private:
	std::unordered_map <int,Ptr<ITracker> > trackers;
	std::unordered_map <int,Rect2d> boundingBoxes;
	std::unordered_map<int,Scalar> colors;
};