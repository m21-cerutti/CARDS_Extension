#pragma once

#include "interface_tracker.h"

#define epsilon 10
#define minArea 10


class TrackerCOLOR : public ITracker
{
public:

	TrackerCOLOR();

	/// @brief Initializes the tracker.
	/// @param image The frame within the object is tracked.
	/// @param boundingBox The bounding box of the object.
	/// @return True if initialization is successfull, false otherwise.
	bool init( InputArray image,const Rect2d& boundingBox ) override;

	/// @brief Convert frame in hsv space, apply threshold and detect contours of the object.
	/// @param imgage The frame within the object is tracked.
	/// @param boundingBox The bounding box of the object.
	/// @return True if object is detected, false otherwise.
	bool update( InputArray image,Rect2d& boundingBox ) override;

	/// @brief A clean deletion of the tracker.
	void clear() override;

	/// @brief Instatiates the tracker.
	/// @return Pointer to the tracker.
	static Ptr<ITracker> create();

private:

	Mat bgr_frame;
	Mat hsv_frame;
	Mat thresh_frame;

	short color = 0;

	/// @brief Build a color histogram in HSV space to find the most present color within the frame.
	/// @param img The HSV frame within the most present present color is searched.
	/// @return The hue component of the most present color in the frame.
	short findColor( Mat img );

	/// @brief Search for the contour with the maximum area.
	/// @param coutours Set of contours you want to compare.
	/// @return The index of the largest contour area.
	int findMaxArea( vector<vector<Point>> contours );
};