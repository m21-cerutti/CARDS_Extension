#include "tracker_api.h"
#include "utilities.h"
#include "multitracker.h"

/* Intern memory */
string trackingAlg = "COLOR";
MultiTrackerCARDS multitrackers;
/// @brief Handle memory space for trackers.
std::vector<bool> occupied_place;

/// @brief Permit to search for the first empty space in memory.
/// @return The index/id of the new object memory.
inline static int FindFirstFreeMemoryTracker()
{
	auto it = std::find_if( occupied_place.begin(),occupied_place.end(),[&]( const auto& val )
							{
								return val == false;
							} );
	return it - occupied_place.begin();
}

void Init( Target* targets,int& nbTarget,const int maxTarget )
{
	if(targets == nullptr)
	{
		throw std::runtime_error( "Error: targets not initialised !" );
	}
	occupied_place.resize( maxTarget,false );
	nbTarget = 0;
}

void Close( Target* targets,int& nbTarget,const int maxTarget )
{
	for(int i = 0; i < maxTarget; i++)
	{
		if(targets[i].state != StateTracker::Undefined)
		{
			UnRegister( targets[i].id,targets,nbTarget );
		}
	}
	occupied_place.clear();
}

int Register( const Frame& frame,const RectStruct& zone,Target* targets,int& nbTarget,const int maxTarget )
{
	Mat img = FrameToCVMat( frame );

	if(targets[nbTarget].state != StateTracker::Undefined)
	{
		// TODO Code error
		throw std::runtime_error( "Error: Already existing ID or init to not Undefined !" );
	}

	int id = FindFirstFreeMemoryTracker();
	targets[id].id = id;
	multitrackers.add( id,img,Rect2dToRectStruct( zone ),createTrackerByName( trackingAlg ) );
	targets[id].state = StateTracker::Live;
	targets[id].rect = zone;

	targets[id].original_size.x = zone.width;
	targets[id].original_size.y = zone.height;

	occupied_place[id] = true;
	nbTarget++;
	return id;
}

void UnRegister( const int id,Target* targets,int& nbTarget )
{
	if(targets[id].state == StateTracker::Undefined)
	{
		throw std::runtime_error( "Error: Free an non valid tracker !" );
	}

	targets[id].id = -1;
	targets[id].state = StateTracker::Undefined;
	multitrackers.remove( id );

	occupied_place[id] = false;
	nbTarget--;
	return;
}

void Detect( const Frame& frame,const RectStruct& zoneDetection,Target* targets,int& nbTarget,const int maxTarget )
{
	Mat img = FrameToCVMat( frame );
	//TODO
	return;
}

void CheckTrack( const Frame& frame,Target* targets,const int nbTarget )
{
	Mat img = FrameToCVMat( frame );

	//TODO Detect and verify already targets, correct errors, handle occlusion and also put tracker of occluded on the top object.

	return;

}

void Track( const Frame& frame,Target* targets,const int nbTarget )
{
	Mat img = FrameToCVMat( frame );

	//Update Live only
	for(int i = 0; i < nbTarget; i++)
	{
		if(targets[i].state == StateTracker::Live)
		{
			if(!multitrackers.update( targets[i].id,img ))
			{
				std::cerr << "Object with id " + to_string( targets[i].id ) + " is lost." << endl;
				targets[i].state = StateTracker::Lost;
			}
			targets[i].rect = Rect2dToRectStruct( multitrackers.getBoundinBox( targets[i].id ) );
		}
	}
}

Matrix4x4f EstimatePose( const Target& target,const PoseParameters& params )
{
	double Cx = params.intrinsic_camera.c_02;
	double Cy = params.intrinsic_camera.c_12;
	double f = params.intrinsic_camera.c_00;

	double Xscreen = (target.rect.x + target.rect.width / 2.0);
	double Yscreen = (target.rect.y + target.rect.height / 2.0);

	double WpixRatio = target.original_size.x / (1.0 * target.rect.width);
	double HpixRatio = target.original_size.y / (1.0 * target.rect.height);

	// Coordinates
	//float X = (Xscreen-Cx) * params.meter_pixel_x_ratio * f;
	//float Y = (Yscreen-Cy) * params.meter_pixel_y_ratio * f;

	double Z = (((WpixRatio * 1.0 * params.meter_pixel_x_ratio) + (HpixRatio * 1.0 * params.meter_pixel_y_ratio)) / 2.0);

	Matrix4x4f pose;
	pose.c_03 = Xscreen;
	pose.c_13 = Yscreen;
	pose.c_23 = Z;

	//TODO ROTATION

	//Homogenous
	pose.c_30 = 1;
	pose.c_31 = 1;
	pose.c_32 = 1;
	pose.c_33 = 1;
	return pose;
}
