#include "videoprovider.h"
#include "utilities.h"

VideoProvider::VideoProvider( int width,int height )
	:_frame()
{
	if(!_cap.open( 0,cv::CAP_ANY ))
	{
		throw std::runtime_error( "Can't open camera." );
	}
	InitCameraFrame( width,height );
}

VideoProvider::VideoProvider( string filename,int width,int height )
	:_frame()
{
	if(!_cap.open( filename ))
	{
		throw std::runtime_error( "Can't open file " + filename );
	}
	InitCameraFrame( width,height );
}

VideoProvider::~VideoProvider()
{
	delete _frame.rawData;
	_cap.release();
}

const Frame& VideoProvider::GetFrame()
{
	Mat mat;
	_cap >> mat;
	if(mat.empty())
	{
		cerr << "Empty frame camera.";
		_frame.rawData = nullptr;
		return _frame;
	}
	CVMatToFrameRawData( mat,_frame );
	return _frame;
}

void VideoProvider::InitCameraFrame( int width,int height )
{
	_cap.set( CAP_PROP_BUFFERSIZE,3 );
	_cap.set( CAP_PROP_FRAME_WIDTH,width );
	_cap.set( CAP_PROP_FRAME_HEIGHT,height );

	_frame.height = _cap.get( CAP_PROP_FRAME_HEIGHT );
	_frame.width = _cap.get( CAP_PROP_FRAME_WIDTH );
	_frame.rawData = new Color32[(size_t)_frame.height * _frame.width];
}
