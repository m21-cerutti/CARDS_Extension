#include "tracker_color.h"
#include "utilities.h"
#include "opencv2/highgui.hpp"


TrackerCOLOR::TrackerCOLOR()
	: bgr_frame(),hsv_frame(),thresh_frame(),color( 0 )
{
}

short TrackerCOLOR::findColor( Mat img )
{
	img = img( Rect( 0.25 * img.cols,0.25 * img.rows,0.75 * img.cols,0.75 * img.rows ) );

	if(img.empty())
		return 0;

	int hist[180] = { 0 };

	for(int i = 0; i < img.rows; i++)
	{
		for(int j = 0; j < img.cols; j++)
		{
			hist[img.at<Vec3b>( i,j )[0]] += 1;
		}
	}

	int max = hist[0];
	short res = 0;

	for(int i = 0; i < 180; i++)
	{
		if(hist[i] > max)
		{
			max = hist[i];
			res = i;
		}
	}

	return res;
}

int TrackerCOLOR::findMaxArea(vector<vector<Point>> contours)
{
	double max = 0.0;
	int max_i = 0;

	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		if (area > max) {
			max = area;
			max_i = i;
		}
	}

	return max_i;
}


bool TrackerCOLOR::init( InputArray image,const Rect2d& boundingBox )
{
	bgr_frame = image.getMat();
	if(bgr_frame.empty())
		return false;
	blur( bgr_frame,bgr_frame,Size( 5,5 ) );
	//DebugMat( bgr_frame,"BGR" + color );
	hsv_frame = Mat( bgr_frame.rows,bgr_frame.cols,bgr_frame.type() );
	cvtColor( bgr_frame,hsv_frame,COLOR_BGR2HSV );

	//Mat channel[3];
	//split( hsv_frame,channel );
	//DebugMat( channel[0],"H" );
	thresh_frame = Mat( bgr_frame.rows,bgr_frame.cols,bgr_frame.type() );
	Mat tmp = Mat( hsv_frame,boundingBox );
	//DebugMat( tmp,"rect" );
	color = findColor( tmp );

	return true;
}

bool TrackerCOLOR::update( InputArray image,Rect2d& boundingBox )
{
	bgr_frame = image.getMat();

	if(bgr_frame.empty())
		return false;

	blur( bgr_frame,bgr_frame,Size( 5,5 ) );
	cvtColor( bgr_frame,hsv_frame,COLOR_BGR2HSV );
	vector<vector<Point>> contours;
	Vec3b lower;
	if(color - epsilon < 0)
	{
		lower = Vec3b( 0,50,50 );
	}
	else
	{
		lower = Vec3b( color - epsilon,50,50 );
	}
	Vec3b higher = Vec3b( color + epsilon,225,225 );
	inRange( hsv_frame,lower,higher,thresh_frame );
	Mat tmp_frame = Mat( thresh_frame.rows,thresh_frame.cols,thresh_frame.type() );
	erode( thresh_frame,tmp_frame,getStructuringElement( MORPH_ELLIPSE,Size( 3,3 ) ) );
	dilate( tmp_frame,thresh_frame,getStructuringElement( MORPH_ELLIPSE,Size( 3,3 ) ) );
	findContours( thresh_frame,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE );

	//DebugMat( thresh_frame,"Tresh " + to_string( color ) );

	if(!contours.empty())
	{
		Rect2d rect = boundingRect( contours[findMaxArea(contours)] );
		boundingBox = rect;
	}
//	if(rect.area() < minArea)
//	{
////TODO convert rect into RectStruct (new function ?)
//// garder rect et on prend l'angle avec le rotated
//	}

	return true;
}

void TrackerCOLOR::clear()
{
	bgr_frame = Mat();
	hsv_frame = Mat();
	thresh_frame = Mat();
	color = 0;
}

Ptr<ITracker> TrackerCOLOR::create()
{
	return Ptr<ITracker>( new TrackerCOLOR() );
}
