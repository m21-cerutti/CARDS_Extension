#pragma once

#include "tracker_api.h"

extern "C"
{
	/// @brief Register manually with a ROI selection in OpenCV windows the different object to track.
	/// @param frame The current frame with objects.
	/// @param targets An allocated array of targets.
	/// @param nbTarget The number of targets.
	/// @param maxTarget The maximum number of targets.
	EXPORT_API void __stdcall ManualRegister( const Frame& frame,Target* targets,int& nbTarget,const int maxTarget );

	/// @brief Debug in OpenCV window the actual frame and targets. Green if Live, Red otherwise.
	/// @param frame The current frame with objects.
	/// @param targets An allocated array of targets.
	/// @param maxTarget The number maximum of targets.
	EXPORT_API void __stdcall DebugTargets( const Frame& frame,Target* targets,const int maxTarget );
}