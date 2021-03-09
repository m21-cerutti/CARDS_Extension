#pragma once

#include "tracker_api.h"

extern "C"
{
	// Select ROI register from OpenCV windows
	EXPORT_API void __stdcall ManualRegister( const Frame& frame,Target* targets,int& nbTarget,const int maxTarget );

	// Debug targets
	EXPORT_API void __stdcall DebugTargets( const Frame& frame,Target* targets,const int nbTarget );
}