#include "multitracker.h"

MultiTrackerCARDS::MultiTrackerCARDS()
{
}

MultiTrackerCARDS::~MultiTrackerCARDS()
{
	trackers.clear();
	boundingBoxes.clear();
	colors.clear();
}

void MultiTrackerCARDS::add( const int id,InputArray image,const Rect2d& boundingBox,Ptr<ITracker> tracker_algorithm )
{
	Ptr<ITracker> tracker = tracker_algorithm;
	if(tracker == NULL)
	{
		throw runtime_error( "Error : tacker null." );
	}

	if(!tracker->init( image,boundingBox ))
	{
		throw runtime_error( "Error : tracker can't be initialised." );
	}

	if(trackers.find( id ) != trackers.end())
	{
		throw runtime_error( "Error : id already used " + to_string( id ) );
	}

	pair<int,Ptr<ITracker>> tmp( id,tracker );
	trackers.insert( tmp );

	pair<int,Rect2d> bb( id,boundingBox );
	boundingBoxes.insert( bb );

	pair<int,Scalar> col;
	col.first = id;
	//Assign a random color to target
	if(trackers.size() == 0)
		col.second = Scalar( 0,0,255 );
	else
		col.second = Scalar( rand() % 256,rand() % 256,rand() % 256 );
	colors.insert( col );
}

void MultiTrackerCARDS::remove( const int id )
{
	auto it = trackers.find( id );
	if(it == trackers.end())
	{
		throw runtime_error( "Error : id not used." );
	}
	trackers.erase( it );
}

bool MultiTrackerCARDS::update( const int id,InputArray image )
{
	return trackers[id]->update( image,boundingBoxes[id] );
}

Rect2d MultiTrackerCARDS::getBoundinBox( const int id )
{
	return boundingBoxes[id];
}

Scalar MultiTrackerCARDS::getColor( const int id )
{
	return colors[id];
}
