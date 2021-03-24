#pragma once

#include "tracker_api.h"

extern "C"
{
	/// @brief Get the calibration parameters.
	/// To create those parameters, use the python script 'camera_calibration/create_data_calib.py', then use 'camera_calibration/calibration.py'.
	/// @param resourceDirectory Folder where python script saved data.
	/// @param parameters Parameters of the camera.
	/// @return False if the folder doesn't exist, true otherwise.
	EXPORT_API bool __stdcall GetPoseParameters( const char* resourceDirectory,PoseParameters& parameters );
}