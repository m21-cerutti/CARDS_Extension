#include "utilities_api.h"
#include "utilities.h"

void ManualRegister( const Frame& frame,Target* targets,int& nbTarget,const int maxTarget )
{
	if(targets == nullptr)
		return;

	Mat img = FrameToCVMat( frame );

	vector<Rect> ROIs;
	selectROIs( "select ROI",img,ROIs );
	if(ROIs.size() < 1)
		return;

	for(Rect rect : ROIs)
	{
		Register( frame,Rect2dToRectStruct( rect ),targets,nbTarget,maxTarget );
	}
}

void DebugTargets( const Frame& frame,Target* targets,const int maxTarget )
{
	if(targets == nullptr)
		return;
	Mat img = FrameToCVMat( frame );
	DebugCVTargets( img,targets,maxTarget );
}
