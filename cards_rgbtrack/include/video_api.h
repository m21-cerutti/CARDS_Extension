#pragma once

#include "videoprovider.h"

extern "C"
{
	/// @brief Create camera context.
	/// @param width 
	/// @param height 
	/// @return 
	EXPORT_API VideoProvider* __stdcall CreateCameraContext( int width,int height );

	/// @brief 
	/// @param filename 
	/// @param width 
	/// @param height 
	/// @return
	EXPORT_API VideoProvider* __stdcall CreateVideoContext( const char* filename,int width,int height );

	/// @brief 
	/// @param video 
	EXPORT_API void __stdcall FreeVideoContext( VideoProvider* video );

	/// @brief 
	/// @param video 
	/// @param frame 
	/// @return 
	EXPORT_API bool __stdcall GetFrame( VideoProvider* video,Frame& frame );
}