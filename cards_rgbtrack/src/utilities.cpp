#include "utilities.h"
#include "tracker_api.h"

using namespace std;
using namespace cv;

Mat TextureToCVMat( const Frame& frame )
{
	// TODO Need some checks (frame raw_data initialised etc)
	Mat texture( frame.height,frame.width,CV_8UC4,frame.rawData );
	cv::cvtColor( texture,texture,cv::COLOR_RGBA2BGR );
	flip( texture,texture,0 );
	if(frame.is_flipped_y)
	{
		flip( texture,texture,0 );
	}
	if(frame.is_flipped_x)
	{
		flip( texture,texture,1 );
	}
	return texture;
}

RectStruct Rect2dToRectStruct( const Rect2d& rect )
{
	RectStruct rectstruct = {
		(float)rect.x,
		(float)rect.y,
		(float)rect.width,
		(float)rect.height,
	};
	return rectstruct;
}

Rect2d Rect2dToRectStruct( const RectStruct& rect )
{
	Rect2d rect2d( (double)rect.x,
				   (double)rect.y,
				   (double)rect.width,
				   (double)rect.height );
	return rect2d;
}

void DebugMat( const Mat& mat )
{
	imshow( "DebugOpenCV",mat );
	waitKey( 25 );
}

void DebugCVTargets( const Mat& mat,const std::vector<pair<Rect2d,Scalar>>& objects,int number )
{
	Mat img = mat.clone();
	for(int i = 0; i < number; i++)
	{
		cv::rectangle( img,objects[i].first,objects[i].second,2,cv::LINE_8 );
	}
	DebugMat( img );
}

void DebugCVTargets( const Mat& mat,const Target* targets,int number )
{
	if(targets == nullptr)
		throw new std::runtime_error( "Empty target list debug." );

	std::vector<pair<Rect2d,Scalar>> objects;
	for(int i = 0; i < number; i++)
	{
		objects.push_back( pair<Rect2d,Scalar>(
			Rect2dToRectStruct( targets[i].rect ),
			targets[i].state == StateTracker::Live ? Scalar( 0,255,0 ) : Scalar( 0,0,255 ) )
		);
	}

	DebugCVTargets( mat,objects,number );
}

Ptr<Tracker> createTrackerByName( const std::string& name )
{
	using namespace cv;

	cv::Ptr<cv::Tracker> tracker;

	if(name == "KCF")
		tracker = TrackerKCF::create();
	else if(name == "TLD")
		tracker = TrackerTLD::create();
	else if(name == "BOOSTING")
		tracker = TrackerBoosting::create();
	else if(name == "MEDIAN_FLOW")
		tracker = TrackerMedianFlow::create();
	else if(name == "MIL")
		tracker = TrackerMIL::create();
	else if(name == "GOTURN")
		tracker = TrackerGOTURN::create();
	else if(name == "MOSSE")
		tracker = TrackerMOSSE::create();
	else if(name == "CSRT")
		tracker = TrackerCSRT::create();
	else
		CV_Error( cv::Error::StsBadArg,"Invalid tracking algorithm name\n" );

	return tracker;
}
