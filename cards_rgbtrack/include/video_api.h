#pragma once

#include "videoprovider.h"

extern "C"
{
	/// @brief Create camera context VideoProvider.
	/// @param width The requested width 
	/// @param height The requested height
	/// @return A pointer to the VideoProvider allocated.
	EXPORT_API VideoProvider* __stdcall CreateCameraContext( int width,int height );

	/// @brief Create video context VideoProvider.
	/// @param filename The path of the video.
	/// @param width The requested width. 
	/// @param height The requested height.
	/// @return A pointer to the VideoProvider allocated.
	EXPORT_API VideoProvider* __stdcall CreateVideoContext( const char* filename,int width,int height );

	/// @brief Free a VideoProvider allocated.
	/// @param video The VideoProvider.
	EXPORT_API void __stdcall FreeVideoContext( VideoProvider* video );

	/// @brief Get a frame from the media open in the VideoProvider.
	/// @param video The VideoProvider.
	/// @param frame The frame to set.
	/// @return True if suceed, false otherwise.
	EXPORT_API bool __stdcall GetFrame( VideoProvider* video,Frame& frame );

	/// @brief Free frame data. Use only when a manual copy have been done with GetCopyFrame.
	/// @param frame to free.
	EXPORT_API void __stdcall FreeCopyFrame( Frame& frame );

	/// @brief Copy the source frame to destination.
	/// @param src the frame to copy.
	/// @param dst the destination frame.
	/// @return True if suceed, false otherwise.
	EXPORT_API bool __stdcall GetCopyFrame( const Frame& src,Frame& dst );
}