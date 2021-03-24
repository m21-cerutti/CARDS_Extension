#include "utilities.h"
#include "tracker_api.h"
#include "tracker_color.h"

using namespace std;
using namespace cv;

Mat FrameToCVMat( const Frame& src )
{
	if (src.rawData == NULL || src.height == 0 || src.width == 0) {
		return Mat();
	}
	Mat mat( src.height,src.width,CV_8UC4,src.rawData );
	cvtColor( mat,mat,cv::COLOR_RGBA2BGR );
	flip( mat,mat,0 );
	int flip_code = (int)src.flip_mode - 2;
	if(flip_code >= -1)
	{
		flip( mat,mat,flip_code );
	}
	return mat;
}

void CVMatToFrameRawData( const Mat& src,Frame& dst )
{
	Mat img = src.clone();
	cvtColor( img,img,CV_BGR2RGBA );
	flip( img,img,0 );
	int flip_code = (int)dst.flip_mode - 2;
	if(flip_code >= -1)
	{
		flip( src,src,flip_code );
	}
	memcpy( dst.rawData,img.data,img.channels() * img.total() );
}

void CopyFrame( const Frame& src,Frame& dst )
{
	Mat mat = FrameToCVMat( src );
	Mat matCopy;
	mat.copyTo( matCopy );
	CVMatToFrameRawData( matCopy,dst );
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

Mat Matrix3x3fToMat( const Matrix3x3f& mat )
{
	float data[9] = {
		mat.c_00,mat.c_01,mat.c_02,
		mat.c_10,mat.c_11,mat.c_12,
		mat.c_20,mat.c_21,mat.c_22
	};
	Mat matcv( Size( 3,3 ),CV_32FC1,data );
	return matcv.clone();
}

Matrix3x3f MatToMatrix3x3f( const Mat& mat )
{
	Matrix3x3f mat3x3;
	mat3x3.c_00 = mat.at<double>( 0,0 );
	mat3x3.c_01 = mat.at<double>( 0,1 );
	mat3x3.c_02 = mat.at<double>( 0,2 );
	mat3x3.c_10 = mat.at<double>( 1,0 );
	mat3x3.c_11 = mat.at<double>( 1,1 );
	mat3x3.c_12 = mat.at<double>( 1,2 );
	mat3x3.c_20 = mat.at<double>( 2,0 );
	mat3x3.c_21 = mat.at<double>( 2,1 );
	mat3x3.c_22 = mat.at<double>( 2,2 );
	return mat3x3;
}

void DebugMat( const Mat& mat,const string win_name )
{
	imshow( win_name,mat );
	waitKey( 25 );
}

void DebugCVTargets( const Mat& mat,const Target* targets,const int maxTarget )
{
	if(targets == nullptr)
		throw new std::runtime_error( "Empty target list debug." );

	Mat img = mat.clone();
	for(int i = 0; i < maxTarget; i++)
	{
		if(targets[i].state != StateTracker::Undefined)
		{
			cv::rectangle( img,
						   Rect2dToRectStruct( targets[i].rect ),
						   targets[i].state == StateTracker::Live ? Scalar( 0,255,0 ) : Scalar( 0,0,255 ),
						   2,
						   cv::LINE_8 );
		}
	}
	DebugMat( img );
}

Ptr<ITracker> createTrackerByName( const std::string& name )
{
	using namespace cv;

	cv::Ptr<ITracker> tracker;

	if(name == "KCF")
		tracker = CardsKCF::create();
	else if(name == "TLD")
		tracker = CardsTLD::create();
	else if(name == "BOOSTING")
		tracker = CardsBoosting::create();
	else if(name == "MEDIAN_FLOW")
		tracker = CardsMedianFlow::create();
	else if(name == "MIL")
		tracker = CardsMIL::create();
	/*else if(name == "GOTURN")
		tracker = CardsGOTURN::create();*/
	else if(name == "MOSSE")
		tracker = CardsMOSSE::create();
	else if(name == "CSRT")
		tracker = CardsCSRT::create();
	else if(name == "COLOR")
		tracker = TrackerCOLOR::create();
	else
		CV_Error( cv::Error::StsBadArg,"Invalid tracking algorithm name\n" );

	return tracker;
}
