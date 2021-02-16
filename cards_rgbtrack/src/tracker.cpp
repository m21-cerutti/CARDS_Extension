#include "tracker.h"
#include "utilitaries.h"

MultiTracker trackers;

int Init()
{
	return 0;
}

void Close()
{

}

void Detect(Frame& frame, Target* targets, int& nbTarget, int maxTarget)
{
	Mat img = TextureToCVMat(frame);
}

void Track(Frame& frame, Target* targets, int nbTarget)
{
	Mat img = TextureToCVMat(frame);
	trackers.update(img);
}
