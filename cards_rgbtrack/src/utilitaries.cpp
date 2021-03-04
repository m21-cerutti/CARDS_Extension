#include "utilitaries.h"
#include "tracker.h"

using namespace std;
using namespace cv;

Mat TextureToCVMat( const Frame& frame )
{
	// TODO Need some checks (frame initialised)
	Mat texture( frame.height,frame.width,CV_8UC4,frame.rawData );
	cv::cvtColor( texture,texture,cv::COLOR_RGBA2BGR );
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
	cv::imshow( "DebugOpenCV",mat );
	cv::waitKey( 25 );
}

void DebugCVTargets( const Mat& mat,const std::vector<Rect2d>& objects,int number )
{
	Mat img = mat.clone();
	for(int i = 0; i < number; i++)
	{
		cv::rectangle( img,objects[i],Scalar( 255,0,0 ),2,cv::LINE_8 );
	}
	DebugMat( img );
}

void DebugCVTargets( const Mat& mat,const Target* targets,int number )
{
	if(targets == nullptr)
		throw new std::runtime_error( "Empty target list debug." );

	std::vector<Rect2d> objects;
	for(int i = 0; i < number; i++)
	{
		objects.push_back( Rect2dToRectStruct( targets[i].rect ) );
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
