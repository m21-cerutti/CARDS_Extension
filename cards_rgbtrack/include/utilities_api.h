#pragma once

#include "tracker_api.h"

extern "C"
{
	/// @brief 
	/// @param frame 
	/// @param targets 
	/// @param nbTarget 
	/// @param maxTarget 
	EXPORT_API void __stdcall ManualRegister( const Frame& frame,Target* targets,int& nbTarget,const int maxTarget );

	/// @brief 
	/// @param frame 
	/// @param targets 
	/// @param nbTarget 
	EXPORT_API void __stdcall DebugTargets( const Frame& frame,Target* targets,const int nbTarget );
}