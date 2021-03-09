#include "tracker_api.h"
#include "utilities.h"
#include "multitracker.h"

/* Intern memory */
String trackingAlg = "MOSSE";
MultiTrackerCARDS multitrackers;
std::vector<bool> free_place;

inline static int FindFirstFreeMemoryTracker()
{
	auto it = std::find_if( free_place.begin(),free_place.end(),[&]( const auto& val )
							{
								return val == false;
							} );
	return it - free_place.begin();
}

void Init( Target* targets,int& nbTarget,const int maxTarget )
{
	if(targets == nullptr)
	{
		throw std::runtime_error( "Error: targets not initialised !" );
	}
	free_place.resize( maxTarget,false );
	nbTarget = 0;
}

void Close( Target* targets,int& nbTarget,const int maxTarget )
{
	int tmp = nbTarget;
	for(int i = 0; i < tmp; i++)
	{
		UnRegister( targets[i].ID,targets,nbTarget );
	}
	free_place.clear();
}

void Register( const Frame& frame,const RectStruct& zone,Target* targets,int& nbTarget,const int maxTarget )
{
	Mat img = TextureToCVMat( frame );

	if(targets[nbTarget].state != StateTracker::Undefined)
	{
		throw std::runtime_error( "Error: Already existing ID or init to not Undefined !" );
	}

	int id = FindFirstFreeMemoryTracker();
	targets[id].ID = id;
	multitrackers.add( id,img,Rect2dToRectStruct( zone ),createTrackerByName( trackingAlg ) );
	targets[id].state = StateTracker::Live;
	targets[id].rect = zone;

	free_place[id] = true;
	nbTarget++;
}

void UnRegister( const int id,Target* targets,int& nbTarget )
{
	if(targets[id].state == StateTracker::Undefined)
	{
		throw std::runtime_error( "Error: Free an non valid tracker !" );
	}

	targets[id].ID = -1;
	targets[id].state = StateTracker::Undefined;
	multitrackers.remove( id );

	free_place[id] = false;
	nbTarget--;
	return;
}

void Detect( const Frame& frame,const RectStruct& zoneDetection,Target* targets,int& nbTarget,const int maxTarget )
{
	Mat img = TextureToCVMat( frame );
	//TODO
	return;
}

void CheckTrack( const Frame& frame,Target* targets,const int nbTarget )
{
	Mat img = TextureToCVMat( frame );

	//TODO Detect and verify already targets, correct errors, handle occlusion and also put tracker of occluded on the top object.

	return;

}

void Track( const Frame& frame,Target* targets,const int nbTarget )
{
	Mat img = TextureToCVMat( frame );
	//TODO preprocess to gray ? Color treshold ? etc.

	//Update Live only
	for(int i = 0; i < nbTarget; i++)
	{
		if(targets[i].state == StateTracker::Live)
		{
			if(!multitrackers.update( targets[i].ID,img ))
			{
				std::cerr << "Object with id " + to_string( targets[i].ID ) + " is lost." << endl;
				targets[i].state = StateTracker::Lost;
			}
			targets[i].rect = Rect2dToRectStruct( multitrackers.getBoundinBox( targets[i].ID ) );
		}
	}
}
