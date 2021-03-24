#include "video_api.h"
#include "utilities.h"

VideoProvider* CreateCameraContext( int width,int height )
{
	VideoProvider* video = new VideoProvider( width,height );
	return video;
}

VideoProvider* CreateVideoContext( const char* filename,int width,int height )
{
	VideoProvider* video = new VideoProvider( string( filename ),width,height );
	return video;
}

void FreeVideoContext( VideoProvider* video )
{
	if(video == nullptr)
		return;
	delete video;
}

bool GetFrame( VideoProvider* video,Frame& frame )
{
	if(video == nullptr)
		return false;

	frame = video->GetFrame();
	if(frame.rawData != nullptr)
	{
		return true;
	}
	return false;
}

void FreeCopyFrame( Frame& frame )
{
	if(frame.rawData == nullptr)
		return;
	delete frame.rawData;
}

bool GetCopyFrame( const Frame& src,Frame& dst )
{
	if(src.rawData == nullptr)
		return false;
	CopyFrame( src,dst );
	return true;
}
