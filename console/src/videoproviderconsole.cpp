#include "videoproviderconsole.h"

VideoProviderConsole::VideoProviderConsole()
{
	_frameBackground.rawData = nullptr;
	_frame.rawData = nullptr;
	if(!_cap.open( 0,CAP_DSHOW ))
	{
		throw std::runtime_error( "Can't open camera." );
	}

	_cap.set( CAP_PROP_FRAME_WIDTH,256 );
	_cap.set( CAP_PROP_FRAME_HEIGHT,256 );
}

VideoProviderConsole::VideoProviderConsole( string filename )
{
	delete _frameBackground.rawData;
	delete _frame.rawData;
	if(!_cap.open( filename ))
	{
		throw std::runtime_error( "Can't open file " + filename );
	}

	_cap.set( CAP_PROP_FRAME_WIDTH,256 );
	_cap.set( CAP_PROP_FRAME_HEIGHT,256 );
}

VideoProviderConsole::~VideoProviderConsole()
{
	free(_frame.rawData);
	free(_frameBackground.rawData );
	_cap.release();
}

const Frame& VideoProviderConsole::GetFrame(bool isBackground)
{
	Mat mat, mat1, mat2;
	_cap >> mat;
	if (isBackground == false)
	{
		if (mat.empty())
		{
			cerr << "Empty frame camera.";
			_frame.rawData = nullptr;
			return _frame;
		}
		if (_frame.rawData == nullptr)
		{
			InitCameraFrame(mat.cols, mat.rows);
		}
		mat.copyTo(mat1);
		cvtColor(mat1, mat1, CV_BGR2RGBA);
		memcpy(_frame.rawData, mat1.data, 4 * mat1.total());
		//Due to Unity space
		_frame.flip_mode = FlipMode::FlipY;
		return _frame;
	}
	else {
		if (mat.empty())
		{
			cerr << "Empty frame camera.";
			_frameBackground.rawData = nullptr;
			return _frameBackground;
		}
		if (_frameBackground.rawData == nullptr)
		{
			InitCameraFrame(mat.cols, mat.rows);
		}
		mat.copyTo(mat2);
		cvtColor(mat2, mat2, CV_BGR2RGBA);
		memcpy(_frameBackground.rawData, mat2.data, 4 * mat2.total());
		//Due to Unity space
		_frameBackground.flip_mode = FlipMode::FlipY;
		return _frameBackground;
	}
	
}

void VideoProviderConsole::InitCameraFrame( int width,int height )
{
	_frame.height = height;
	_frame.width = width;
	_frame.rawData = new Color32[(size_t)height * width];
	_frameBackground.height = height;
	_frameBackground.width = width;
	_frameBackground.rawData = new Color32[(size_t)height * width];
}
