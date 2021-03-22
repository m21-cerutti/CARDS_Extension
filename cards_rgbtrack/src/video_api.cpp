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

void FreeFrame( Frame& frame )
{
	if (frame.rawData == nullptr)
		return;
	delete frame.rawData;
}

Frame GetCopyFrame( const Frame& src )
{
	Frame dst;
	dst.width = src.width;
	dst.height = src.height;
	dst.rawData = new Color32[(size_t)dst.height * dst.width];
	CopyFrame(src, dst);
	return dst;
}
