#include "tracker.h"
#include "utilitaries.h"

/* Intern memory */
String trackingAlg = "MOSSE";
MultiTracker trackers;
int ID_PROVIDER = 1;

void ManualRegister( const Frame& frame,Target* targets,int& nbTarget,const int maxTarget )
{
	Mat img = TextureToCVMat( frame );

	//TODO to fill with rect of new objects 
	vector<Rect> ROIs;
	//TODO to delete (using for test, after set it to size of objects automatically)
	selectROIs( "tracker",img,ROIs );

	//quit when the tracked object(s) is not provided
	if(ROIs.size() < 1)
		return;

	int i = 0;
	for(Rect rect : ROIs)
	{
		Register( i,frame,Rect2dToRectStruct( rect ),targets,nbTarget,maxTarget );
		i++;
	}
}

void DebugTargets( const Frame& frame,Target* targets,const int nbTarget )
{
	Mat img = TextureToCVMat( frame );
	DebugCVTargets( img,targets,nbTarget );
}

int Init()
{
	return 0;
}

void Close()
{
	return;
}

bool Register( int id,const Frame& frame,const RectStruct& zone,Target* targets,int& nbTarget,const int maxTarget )
{
	Mat img = TextureToCVMat( frame );

	//quit when the tracked object(s) is not provided
	if(nbTarget + 1 > maxTarget)
	{
		std::cerr << "Error: Limit of tracking object reach !" << endl;
		return false;
	}

	if(targets[id].state != StateTracker::Undefined)
	{
		std::cerr << "Error: Already existing ID !" << endl;
		return false;
	}

	//TODO internal structure to check free place instead of ID ?
	targets[id].ID = id;
	targets[id].state = StateTracker::Live;
	targets[id].rect = zone;
	trackers.add( createTrackerByName( trackingAlg ),img,Rect2dToRectStruct( zone ) );

	nbTarget++;

	//TODO Return ID ?
	return true;
}

void UnRegister( int id,Target* targets,int& nbTarget )
{
	//TODO
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

	//TODO Update only some trackers ? Need to reimplement multitracker in this case.
	trackers.update( img );
	for(unsigned i = 0; i < trackers.getObjects().size(); i++)
	{
		if(targets[i].state == StateTracker::Live)
		{
			targets[i].rect = Rect2dToRectStruct( trackers.getObjects()[i] );
		}
	}
}
