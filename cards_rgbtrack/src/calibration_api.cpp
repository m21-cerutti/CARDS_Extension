#include "calibration_api.h"
#include "utilities.h"

bool GetPoseParameters( const char* resourceDirectory,PoseParameters& parameters )
{
	FileStorage fs;
	string filepath = string( resourceDirectory ) + "/calibration.xml";
	fs.open( filepath,FileStorage::READ );
	if(!fs.isOpened())
	{
		return false;
	}

	Mat camera_matrix;
	float ratio_x,ratio_y,dist_cam;
	fs["camera_matrix"] >> camera_matrix;
	fs["ratio_x"] >> ratio_x;
	fs["ratio_y"] >> ratio_y;

	fs["dist_cam"] >> dist_cam;

	parameters.dist_cam = dist_cam;
	parameters.pixel_mm_x_ratio = ratio_x;
	parameters.pixel_mm_y_ratio = ratio_y;
	parameters.intrinsic_camera = MatToMatrix3x3f( camera_matrix );

	return true;
}
