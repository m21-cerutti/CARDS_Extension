#include "tracker.h"
#include "utilitaries.h"

/* Intern memory */
String trackingAlg = "MOSSE";
MultiTracker trackers;
int ID_PROVIDER = 0;

int Init()
{
	//TODO init frame memory ?
	return 0;
}

void Close()
{

}

void Detect(Frame& frame, Target* targets, int& nbTarget, int maxTarget)
{
	Mat img = TextureToCVMat(frame);

	//TODO Detect and verify already targets, and also put tracker of occluded on the top object.
	//...

	//TODO to fill with rect of new objects 
	vector<Rect> ROIs;
	//TODO to delete (using for test, after set it to size of objects automatically)
	selectROIs("tracker", img, ROIs);

	//quit when the tracked object(s) is not provided
	if (ROIs.size() < 1)
		return;

	// initialize the tracker of new objects
	for (size_t i = 0; i < ROIs.size(); i++)
	{
		//TODO not effective, need to know available free place in targets.
		targets[nbTarget + i].ID = ID_PROVIDER++;
		targets[nbTarget + i].state = StateTracker::Live;
		targets[nbTarget + i].object = ROIs[i];
		trackers.add(createTrackerByName(trackingAlg), img, targets[nbTarget + i].object);
	}
	nbTarget += ROIs.size();

}

void Track(Frame& frame, Target* targets, int nbTarget)
{
	Mat img = TextureToCVMat(frame);
	//TODO preprocess to gray ? Color treshold ? etc.

	//TODO Update only some trackers ? Need to reimplement multitracker in this case.
	trackers.update(img);
	for (unsigned i = 0; i < trackers.getObjects().size(); i++)
	{
		if (targets[i].state == StateTracker::Live)
		{
			Rect2d object = trackers.getObjects()[i];
			targets[i].object = object;
		}
	}
	if (trackers.getObjects().size() > 0)
		DebugTargets(img, trackers.getObjects(), trackers.getObjects().size());
}
