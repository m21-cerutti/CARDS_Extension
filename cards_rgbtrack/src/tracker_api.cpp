#include "tracker_api.h"
#include "utilities.h"
#include "multitracker.h"

/* Intern memory */

/// @brief Main tracking algorithm
string trackingAlg = "MOSSE";
MultiTrackerCARDS multitrackers;

/// @brief Use to improve quality
string trackingDetector = "COLOR";
MultiTrackerCARDS detectors;

/// @brief Handle memory space for trackers.
std::vector<bool> occupied_place;

/// @brief Permit to search for the first empty space in memory.
/// @return The index/id of the new object memory.
inline static int FindFirstFreeMemoryTracker()
{
	auto it = std::find_if( occupied_place.begin(),occupied_place.end(),[&]( const auto& val )
							{
								return val == false;
							} );
	return it - occupied_place.begin();
}

void Init( Target* targets,int& nbTarget,const int maxTarget )
{
	if(targets == nullptr)
	{
		cerr << "Error: targets not initialised !" << endl;
		return;
	}
	occupied_place.resize( maxTarget,false );
	nbTarget = 0;
}

void Close( Target* targets,int& nbTarget,const int maxTarget )
{
	if(targets != NULL)
	{
		for(int i = 0; i < maxTarget; i++)
		{
			if(targets[i].state != StateTracker::Undefined)
			{
				UnRegister( targets[i].id,targets,nbTarget );
			}
		}
		occupied_place.clear();
	}
}

int Register( const Frame& frame,const RectStruct& zone,Target* targets,int& nbTarget,const int maxTarget )
{
	Mat img = FrameToCVMat( frame );

	if(targets[nbTarget].state != StateTracker::Undefined ||
		img.empty() || targets == NULL || zone.height == 0 || zone.width == 0)
	{
		cerr << "Error: Already existing ID or init to not Undefined !" << endl;
		return -1;
	}

	int id = FindFirstFreeMemoryTracker();
	targets[id].id = id;
	multitrackers.add( id,img,Rect2dToRectStruct( zone ),createTrackerByName( trackingAlg ) );
	detectors.add( id,img,Rect2dToRectStruct( zone ),createTrackerByName( trackingDetector ) );
	targets[id].state = StateTracker::Live;
	targets[id].rect = zone;

	targets[id].original_size.x = zone.width;
	targets[id].original_size.y = zone.height;

	occupied_place[id] = true;
	nbTarget++;
	return id;
}

void UnRegister( const int id,Target* targets,int& nbTarget )
{
	if(targets[id].state == StateTracker::Undefined || targets == NULL)
	{
		cerr << "Error: Free an non valid tracker !" << endl;
		return;
	}

	targets[id].id = -1;
	targets[id].state = StateTracker::Undefined;
	multitrackers.remove( id );
	detectors.remove( id );

	occupied_place[id] = false;
	nbTarget--;
	return;
}

void Detect( const Frame& frame,const Frame& frameBackground,const RectStruct& zoneDetection,Target* targets,int& nbTarget,const int maxTarget )
{
	if(frame.height == 0 || frame.width == 0 || frame.rawData == nullptr)
	{
		cerr << "Frame is empty" << endl;
		return;
	}
	if(frameBackground.height == 0 || frameBackground.width == 0 || frameBackground.rawData == nullptr)
	{
		cerr << "Background frame is empty" << endl;
		return;
	}

	if(zoneDetection.y < 0 || zoneDetection.x < 0 ||
		zoneDetection.width <= 0 || zoneDetection.height <= 0 ||
		zoneDetection.y + zoneDetection.height > frameBackground.height || zoneDetection.x + zoneDetection.width > frameBackground.width)
	{
		cerr << "Detection zone is not in the image. " << endl;
		return;
	}
	if(targets == nullptr)
	{
		cerr << "Unvalid list of targets" << endl;
		return;
	}

	Mat img = FrameToCVMat( frame );
	Mat background = FrameToCVMat( frameBackground );
	Rect2d zone = Rect2dToRectStruct( zoneDetection );

	//Debug
	Mat debugzoneimg = img.clone();
	rectangle( debugzoneimg,zone,Scalar( 255,0,0 ),5 );
	DebugMat( debugzoneimg,"Detect" );

	Mat zoneImg = img( zone );
	Mat zoneBackground = background( zone );
	Mat imgGray,backgroundGray;
	cvtColor( zoneImg,imgGray,CV_RGB2GRAY );
	cvtColor( zoneBackground,backgroundGray,CV_RGB2GRAY );
	Mat foregroundMask;

	threshold( abs( backgroundGray - imgGray ),foregroundMask,40,255,THRESH_BINARY );
	erode( foregroundMask,foregroundMask,Mat(),Point( -1,-1 ),2,1,1 );
	Mat binaryBackground = foregroundMask;
	foregroundMask.convertTo( foregroundMask,CV_32F );

	Mat tmpImg;
	tmpImg = foregroundMask.mul( foregroundMask );
	Mat imgNorm;
	resize( tmpImg,imgNorm,cv::Size(),(double)200 / tmpImg.cols,(double)200 / tmpImg.rows );

	Scalar s = sum( imgNorm );
	double sse = s.val[0] + s.val[1] + s.val[2];
	double mse = sse / (double)(zoneImg.channels() * zoneImg.total());

	double mseLimit = 500 * (imgNorm.rows * imgNorm.cols) / (tmpImg.rows * tmpImg.cols);

	if(mseLimit < mse)
	{
		vector<Vec4i> hierarchy;
		vector<vector<Point> > contours;
		findContours( binaryBackground,contours,hierarchy,CV_RETR_EXTERNAL,CHAIN_APPROX_SIMPLE );
		vector<vector<Point> > contours_poly( contours.size() );
		vector<Rect> boundRect( contours.size() );
		vector<Point2f>center( contours.size() );
		vector<float>radius( contours.size() );
		RectStruct zoneObject;
		int biggerRect = 0;
		int indexRect = 0;

		for(int i = 0; i < contours.size(); i++)
		{
			approxPolyDP( Mat( contours[i] ),contours_poly[i],3,true );
			boundRect[i] = boundingRect( Mat( contours_poly[i] ) );
			int areaRect = boundRect[i].width * boundRect[i].height;
			if(areaRect > biggerRect)
			{
				biggerRect = areaRect;
				indexRect = i;
			}

		}

		zoneObject = Rect2dToRectStruct( boundRect[indexRect] );

		int indexTarget = 0;
		bool isDetected = false;
		for(int i = 0; i < maxTarget; i++)
		{
			if(targets[i].state == StateTracker::Undefined)
			{
				rectangle( binaryBackground,boundRect[indexRect],Scalar( 255,0,0 ),5 );
				DebugMat( binaryBackground,"Register" );

				Register( frame,zoneObject,targets,nbTarget,maxTarget );
				indexTarget = i;
				isDetected = true;
				break;
			}
		}
	}
	return;
}

void CheckTrack( const Frame& frame,Target* targets,const int maxTarget )
{
	if(frame.height == 0 || frame.width == 0 || frame.rawData == NULL)
	{
		cerr << "Frame is empty" << endl;
		return;
	}
	if(targets == NULL)
	{
		cerr << "Unvalid list of targets" << endl;
		return;
	}
	Mat img = FrameToCVMat( frame );

	for(int i = 0; i < maxTarget; i++)
	{
		if(targets[i].state != StateTracker::Undefined)
		{
			bool tracked = multitrackers.update( targets[i].id,img );
			bool detected = detectors.update( targets[i].id,img );

			Rect2d bbt = multitrackers.getBoundinBox( targets[i].id );
			Rect2d bbd = detectors.getBoundinBox( targets[i].id );

			Vec2f vt = Vec2f( bbt.x + bbt.width / 2.0,bbt.y + bbt.height / 2.0 );
			Vec2f vb = Vec2f( bbd.x + bbd.width / 2.0,bbd.y + bbd.width / 2.0 );
			float dist_track_detect = norm( vb - vt,NORM_L2 );

			if(targets[i].state == StateTracker::Live && tracked && detected)
			{
				if(dist_track_detect > 50.0f)
				{
					tracked = false;
					targets[i].state = StateTracker::Lost;
				}
			}

			if(targets[i].state != StateTracker::Live && !tracked && detected)
			{
				targets[i].state = StateTracker::Live;
				targets[i].rect = Rect2dToRectStruct( bbd );
				//Correct and expand bbd box
				bbt = bbd;
				bbt.x -= 5;
				bbt.y -= 5;
				bbt.width += 10;
				bbt.height += 10;
				multitrackers.remove( targets[i].id );
				multitrackers.add( targets[i].id,img,bbt,createTrackerByName( trackingAlg ) );
			}
			else if(targets[i].state == StateTracker::Occluded && detected && tracked)
			{
				targets[i].state = StateTracker::Live;
				//TODO check all bounding box for occluded, improve with spatial tree
			}
			else if(!detected && !tracked)
			{
				//TODO check last bounding box near limit screen for OutOfCam
			}

			targets[i].rect = Rect2dToRectStruct( multitrackers.getBoundinBox( targets[i].id ) );
		}
	}
}

void Track( const Frame& frame,Target* targets,const int maxTarget )
{
	if(frame.height == 0 || frame.width == 0 || frame.rawData == NULL)
	{
		cerr << "Frame is empty" << endl;
		return;
	}
	if(targets == NULL)
	{
		cerr << "Unvalid list of targets" << endl;
		return;
	}
	Mat img = FrameToCVMat( frame );

	//Update Live only
	for(int i = 0; i < maxTarget; i++)
	{
		if(targets[i].state == StateTracker::Live)
		{
			if(!multitrackers.update( targets[i].id,img ))
			{
				std::cerr << "Object with id " + to_string( targets[i].id ) + " is lost." << endl;
				targets[i].state = StateTracker::Lost;
			}
			targets[i].rect = Rect2dToRectStruct( multitrackers.getBoundinBox( targets[i].id ) );
		}
	}
}

Matrix4x4f EstimatePose( const Target& target,const PoseParameters& params )
{
	double Cx = params.intrinsic_camera.c_02;
	double Cy = params.intrinsic_camera.c_12;
	double f = params.intrinsic_camera.c_00;

	double Xscreen = (target.rect.x + target.rect.width / 2.0);
	double Yscreen = (target.rect.y + target.rect.height / 2.0);

	double WpixRatio = target.original_size.x / (1.0 * target.rect.width);
	double HpixRatio = target.original_size.y / (1.0 * target.rect.height);

	// Coordinates
	//float X = (Xscreen-Cx) * params.meter_pixel_x_ratio * f;
	//float Y = (Yscreen-Cy) * params.meter_pixel_y_ratio * f;

	double Z = (((WpixRatio * 1.0 * params.meter_pixel_x_ratio) + (HpixRatio * 1.0 * params.meter_pixel_y_ratio)) / 2.0);

	Matrix4x4f pose;
	pose.c_03 = Xscreen;
	pose.c_13 = Yscreen;
	pose.c_23 = Z;

	//Homogenous
	pose.c_30 = 1;
	pose.c_31 = 1;
	pose.c_32 = 1;
	pose.c_33 = 1;
	return pose;
}
