#pragma once

#include "interfaceTracker.h"

class TrackerCOLOR : public ITracker 
{
public :
	bool init(InputArray image, const Rect2d& boundingBox) override;
	bool update(InputArray image, Rect2d& boundingBox) override;
	static Ptr<ITracker> create();
};