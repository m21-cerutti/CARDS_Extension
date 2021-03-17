#pragma once

#include  <unordered_map>
#include "opencv2/tracking/tracker.hpp"

#include "interface_tracker.h"

using namespace std;
using namespace cv;

/// @brief MultitrackerClass, based on opencv contributions but retain only the functions signature.
class MultiTrackerCARDS
{
public:
	MultiTrackerCARDS();

	~MultiTrackerCARDS();

	/// @brief Add a tracker to the intern datas.
	/// @param id The ID of the tracker to set, if already used throw an error.
	/// @param image The initial image.
	/// @param boundingBox the initial bounding box.
	/// @param tracker_algorithm the pointer to the tracker to use.
	void add( const int id,InputArray image,const Rect2d& boundingBox,Ptr<ITracker> tracker_algorithm );

	/// @brief Remove a tracker
	/// @param id the id registered, throw an error if not used.
	void remove( const int id );

	/// @brief Update the tracker registered.
	/// @param id
	/// @param image the current image 
	/// @return True if sucessfull, false otherwise.
	bool update( const int id,InputArray image );

	/// @brief Correct and reinitialyze a tracker.
	/// @param id
	/// @param image the current image.
	/// @param boundingBoxe the corrected bounding box.
	void correct( const int id,InputArray image,Rect2d boundingBoxe );

	/// @brief Get the bounding box associated to a tracker.
	/// @param id
	/// @return a Rect2d
	Rect2d getBoundinBox( const int id );

	/// @brief Get the color associated to a tracker.
	/// @param id his ID
	/// @return a Scalar representing RGB color
	Scalar getColor( const int id );

private:
	std::unordered_map <int,Ptr<ITracker> > trackers;
	std::unordered_map <int,Rect2d> boundingBoxes;
	std::unordered_map<int,Scalar> colors;
};