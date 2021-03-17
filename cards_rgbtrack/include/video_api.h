#pragma once

#include "videoprovider.h"

extern "C"
{
	//Camera
	EXPORT_API VideoProvider* __stdcall CreateCameraContext( int width,int height );

	//VideoFile
	EXPORT_API VideoProvider* __stdcall CreateVideoContext( const char* filename,int width,int height );

	EXPORT_API void __stdcall FreeVideoContext( VideoProvider* video );

	//Only fill raw_data of frame
	EXPORT_API bool __stdcall GetFrame( VideoProvider* video,Frame& frame );
}