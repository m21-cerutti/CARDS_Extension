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

/// @brief Interface for all tracker compatible with the MultiTracker class.
__interface ITracker
{
public:
	virtual bool init( InputArray image,const Rect2d& boundingBox );
	virtual bool update( InputArray image,Rect2d& boundingBox );
	virtual void clear();
};

#pragma region Wrapper OpenCV 

class CardsKCF : public ITracker
{
private:
	Ptr<TrackerKCF> tracker;
public:
	CardsKCF()
	{
		tracker = TrackerKCF::create();
	}

	bool init( InputArray image,const Rect2d& boundingBox ) override
	{
		return tracker->init( image,boundingBox );
	}
	bool update( InputArray image,Rect2d& boundingBox ) override
	{
		return tracker->update( image,boundingBox );
	}
	void clear() override
	{
		return tracker->clear();
	}

	static Ptr<ITracker> create()
	{
		return Ptr<ITracker>( new CardsKCF() );
	}

};

class CardsTLD : public ITracker
{
private:
	Ptr<TrackerTLD> tracker;
public:
	CardsTLD()
	{
		tracker = TrackerTLD::create();
	}

	bool init( InputArray image,const Rect2d& boundingBox ) override
	{
		return tracker->init( image,boundingBox );
	}
	bool update( InputArray image,Rect2d& boundingBox ) override
	{
		return tracker->update( image,boundingBox );
	}
	void clear() override
	{
		return tracker->clear();
	}

	static Ptr<ITracker> create()
	{
		return Ptr<ITracker>( new CardsTLD() );
	}
};

class CardsBoosting : public ITracker
{
private:
	Ptr<TrackerBoosting> tracker;
public:
	CardsBoosting()
	{
		tracker = TrackerBoosting::create();
	}

	bool init( InputArray image,const Rect2d& boundingBox ) override
	{
		return tracker->init( image,boundingBox );
	}
	bool update( InputArray image,Rect2d& boundingBox ) override
	{
		return tracker->update( image,boundingBox );
	}
	void clear() override
	{
		return tracker->clear();
	}

	static Ptr<ITracker> create()
	{
		return Ptr<ITracker>( new CardsBoosting() );
	}
};

class CardsMedianFlow : public ITracker
{
private:
	Ptr<TrackerMedianFlow> tracker;
public:
	CardsMedianFlow()
	{
		tracker = TrackerMedianFlow::create();
	}

	bool init( InputArray image,const Rect2d& boundingBox ) override
	{
		return tracker->init( image,boundingBox );
	}
	bool update( InputArray image,Rect2d& boundingBox ) override
	{
		return tracker->update( image,boundingBox );
	}
	void clear() override
	{
		return tracker->clear();
	}

	static Ptr<ITracker> create()
	{
		return Ptr<ITracker>( new CardsMedianFlow() );
	}
};

class CardsMIL : public ITracker
{
private:
	Ptr<TrackerMIL> tracker;
public:
	CardsMIL()
	{
		tracker = TrackerMIL::create();
	}

	bool init( InputArray image,const Rect2d& boundingBox ) override
	{
		return tracker->init( image,boundingBox );
	}
	bool update( InputArray image,Rect2d& boundingBox ) override
	{
		return tracker->update( image,boundingBox );
	}
	void clear() override
	{
		return tracker->clear();
	}

	static Ptr<ITracker> create()
	{
		return Ptr<ITracker>( new CardsMIL() );
	}
};

class CardsGOTURN : public ITracker
{
private:
	Ptr<TrackerGOTURN> tracker;
public:
	CardsGOTURN()
	{
		tracker = TrackerGOTURN::create();
	}

	bool init( InputArray image,const Rect2d& boundingBox ) override
	{
		return tracker->init( image,boundingBox );
	}
	bool update( InputArray image,Rect2d& boundingBox ) override
	{
		return tracker->update( image,boundingBox );
	}
	void clear() override
	{
		return tracker->clear();
	}

	static Ptr<ITracker> create()
	{
		return Ptr<ITracker>( new CardsGOTURN() );
	}
};

class CardsMOSSE : public ITracker
{
private:
	Ptr<TrackerMOSSE> tracker;
public:
	CardsMOSSE()
	{
		tracker = TrackerMOSSE::create();
	}

	bool init( InputArray image,const Rect2d& boundingBox ) override
	{
		return tracker->init( image,boundingBox );
	}
	bool update( InputArray image,Rect2d& boundingBox ) override
	{
		return tracker->update( image,boundingBox );
	}
	void clear() override
	{
		return tracker->clear();
	}

	static Ptr<ITracker> create()
	{
		return Ptr<ITracker>( new CardsMOSSE() );
	}
};

class CardsCSRT : public ITracker
{
private:
	Ptr<TrackerCSRT> tracker;
public:
	CardsCSRT()
	{
		TrackerCSRT::Params params;
		params.scale_lr = 0.2f;
		tracker = TrackerCSRT::create( params );
	}

	bool init( InputArray image,const Rect2d& boundingBox ) override
	{
		return tracker->init( image,boundingBox );
	}
	bool update( InputArray image,Rect2d& boundingBox ) override
	{
		return tracker->update( image,boundingBox );
	}
	void clear() override
	{
		return tracker->clear();
	}

	static Ptr<ITracker> create()
	{
		return Ptr<ITracker>( new CardsCSRT() );
	}

#pragma endregion

};

