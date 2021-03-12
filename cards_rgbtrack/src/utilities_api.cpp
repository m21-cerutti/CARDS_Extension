#include "utilities_api.h"
#include "utilities.h"

void ManualRegister( const Frame& frame,Target* targets,int& nbTarget,const int maxTarget )
{
	Mat img = FrameToCVMat( frame );

	//TODO to fill with rect of new objects 
	vector<Rect> ROIs;
	//TODO to delete (using for test, after set it to size of objects automatically)
	selectROIs( "tracker",img,ROIs );

	//quit when the tracked object(s) is not provided
	if(ROIs.size() < 1)
		return;

	for(Rect rect : ROIs)
	{
		Register( frame,Rect2dToRectStruct( rect ),targets,nbTarget,maxTarget );
	}
}

void DebugTargets( const Frame& frame,Target* targets,const int nbTarget )
{
	Mat img = FrameToCVMat( frame );
	DebugCVTargets( img,targets,nbTarget );
}
