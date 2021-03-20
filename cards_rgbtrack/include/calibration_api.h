#pragma once

#include "tracker_api.h"

extern "C"
{
	// TODO
	EXPORT_API bool __stdcall GetPoseParameters( const char* resourceDirectory,PoseParameters& parameters );
}