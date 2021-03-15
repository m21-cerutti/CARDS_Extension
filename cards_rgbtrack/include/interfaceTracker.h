#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

__interface ITracker
{
public :
	virtual bool init(InputArray image, const Rect2d& boundingBox);
	virtual bool update(InputArray image, Rect2d& boundingBox);
};

class CardsKCF : public ITracker
{
private:
	Ptr<TrackerKCF> tracker;
public:
	CardsKCF() {
		tracker = TrackerKCF::create();
	}

	bool init(InputArray image, const Rect2d& boundingBox) override {
		return tracker->init(image, boundingBox);
	}
	bool update(InputArray image, Rect2d& boundingBox) override;

	static Ptr<ITracker> create() {
		return Ptr<ITracker>(new CardsKCF());
	}
};