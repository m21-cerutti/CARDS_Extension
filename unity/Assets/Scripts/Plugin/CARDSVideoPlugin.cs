using System;

using System.Runtime.InteropServices;

using fts;

namespace Plugin
{
	/// <summary>
	/// Plugin wrapper functions for video.
	/// </summary>
	[PluginAttr("cards_rgbtrack")]
	public static class CARDSVideoPlugin
	{
#if UNITY_EDITOR

		[PluginFunctionAttr("CreateCameraContext")]
		public static CreateCameraContext create_camera_context = null;
		public delegate IntPtr CreateCameraContext(int width, int height);

		[PluginFunctionAttr("CreateVideoContext")]
		public static CreateVideoContext create_video_context = null;
		public delegate IntPtr CreateVideoContext(string filename, int width, int height);

		[PluginFunctionAttr("FreeVideoContext")]
		public static FreeVideoContext free_video_context = null;
		public delegate void FreeVideoContext(IntPtr video);

		[PluginFunctionAttr("GetFrame")]
		public static GetFrame get_frame = null;
		public delegate bool GetFrame(IntPtr video, ref Frame frame);

		[PluginFunctionAttr("GetCopyFrame")]
		public static GetCopyFrame get_copy_frame = null;
		public delegate Frame GetCopyFrame(ref Frame src);

		[PluginFunctionAttr("FreeFrame")]
		public static FreeFrame free_frame = null;
		public delegate void FreeFrame(ref Frame frame);

#else
        [DllImport("cards_rgbtrack")]
		internal static extern IntPtr CreateCameraContext(int width, int height);

		[DllImport("cards_rgbtrack")]
		internal static extern IntPtr CreateVideoContext(string filename, int width, int height);

		[DllImport("cards_rgbtrack")]
		internal static extern void FreeVideoContext(IntPtr video);

		[DllImport("cards_rgbtrack")]
		internal static extern bool GetFrame(IntPtr video, ref Frame frame);

		[DllImport("cards_rgbtrack")]
		internal static extern Frame GetCopyFrame(ref Frame src);

		[DllImport("cards_rgbtrack")]
		internal static extern void FreeFrame(ref Frame frame);
#endif
		// The plugin methods are wrapped in order to be transparent for the users when they are being executed in 
		// editor or standalone mode.
		#region Wrapped methods

		public static IntPtr CreateCameraContextWrapped(int width, int height)
		{
#if UNITY_EDITOR
			return create_camera_context(width, height);
#else
			return CreateCameraContext(width, height);
#endif
		}

		public static IntPtr CreateVideoContextWrapped(string filename, int width, int height)
		{
#if UNITY_EDITOR
			return create_video_context(filename, width, height);
#else
            return CreateVideoContext(filename, width, height);
#endif
		}

		public static void FreeVideoContextWrapped(IntPtr video)
		{
#if UNITY_EDITOR
			free_video_context(video);
#else
            FreeVideoContext(video);
#endif
		}

		public static bool GetFrameWrapped(IntPtr video, ref Frame frame)
		{
#if UNITY_EDITOR
			return get_frame(video, ref frame);
#else
            return GetFrame(video, ref frame);
#endif
		}

		public static Frame GetCopyFrameWrapped(ref Frame frame)
		{
#if UNITY_EDITOR
			return get_copy_frame(ref frame);
#else
            return GetCopyFrame(frame);
#endif
		}

		public static void FreeFrameWrapped(ref Frame frame)
		{
#if UNITY_EDITOR
			get_copy_frame(ref frame);
#else
            GetCopyFrame(frame);
#endif
		}

		#endregion
	}
}