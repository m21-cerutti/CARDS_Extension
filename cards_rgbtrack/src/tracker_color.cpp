#include "tracker_color.h"


TrackerCOLOR::TrackerCOLOR()
{
}

short TrackerCOLOR::findColor( Mat img )
{
	if(img.empty())
		return 0;

	int hist[180] = { 0 };

	for(int i = 0; i < img.cols; i++)
	{
		for(int j = 0; j < img.rows; j++)
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

bool TrackerCOLOR::init( InputArray image,const Rect2d& boundingBox )
{

	bgr_frame = image.getMat();
	if(bgr_frame.empty())
		return false;
	hsv_frame = Mat( bgr_frame.rows,bgr_frame.cols,bgr_frame.type() );
	thresh_frame = Mat( bgr_frame.rows,bgr_frame.cols,bgr_frame.type() );
	color = findColor( hsv_frame );
	return true;
}

bool TrackerCOLOR::update( InputArray image,Rect2d& boundingBox )
{
	cvtColor( bgr_frame,hsv_frame,COLOR_BGR2HSV,0 );
	vector<vector<Point>> contours;
	Vec3b lower = Vec3b( color - delta,100,100 );
	Vec3b higher = Vec3b( color + delta,255,255 );
	inRange( hsv_frame,lower,higher,thresh_frame );
	Mat tmp_frame = Mat( thresh_frame.rows,thresh_frame.cols,thresh_frame.type() );
	erode( thresh_frame,tmp_frame,getStructuringElement( MORPH_ELLIPSE,Size( 3,3 ) ) );
	dilate( tmp_frame,thresh_frame,getStructuringElement( MORPH_ELLIPSE,Size( 3,3 ) ) );
	findContours( thresh_frame,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE );
	Rect rect = boundingRect( contours[0] );
	if(rect.area() < minArea)
	{
//TODO convert rect into RectStruct (new function ?)
// garder rect et on prend l'angle avec le rotated
	}

	return true;
}

Ptr<ITracker> TrackerCOLOR::create()
{
	return Ptr<ITracker>( new TrackerCOLOR() );
}