#include "videoproviderconsole.h"

VideoProviderConsole::VideoProviderConsole()
{
	_frame.rawData = nullptr;
	if(!_cap.open( 0,cv::CAP_ANY ))
	{
		throw std::runtime_error( "Can't open camera." );
	}
}

VideoProviderConsole::VideoProviderConsole( string filename )
{
	delete _frame.rawData;
	if(!_cap.open( filename ))
	{
		throw std::runtime_error( "Can't open file " + filename );
	}
}

VideoProviderConsole::~VideoProviderConsole()
{
	free( _frame.rawData );
	_cap.release();
}

const Frame& VideoProviderConsole::GetFrame()
{
	Mat mat;
	_cap >> mat;
	if(mat.empty())
	{
		cerr << "Empty frame camera.";
		_frame.rawData = nullptr;
		return _frame;
	}
	if(_frame.rawData == nullptr)
	{
		InitCameraFrame( mat.cols,mat.rows );
	}
	cvtColor( mat,mat,CV_BGR2RGBA );
	memcpy( _frame.rawData,mat.data,4 * mat.total() );
	//Due to Unity space
	_frame.flip_mode = FlipMode::FlipY;
	return _frame;
}

void VideoProviderConsole::InitCameraFrame( int width,int height )
{
	_frame.height = height;
	_frame.width = width;
	_frame.rawData = new Color32[(size_t)height * width];
}
