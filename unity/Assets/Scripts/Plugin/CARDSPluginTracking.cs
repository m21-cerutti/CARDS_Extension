using System;
using System.Runtime.InteropServices;

using fts;

namespace Plugin
{

	/// <summary>
	/// Plugin wrapper functions for tracking, contains also data struct that transit between Unity and DLL Plugin.
	/// </summary>

	#region Basic structures

	[System.Serializable]
	public struct Vector3f
	{
		public float x;
		public float y;
		public float z;
	};

	[System.Serializable]
	public struct Matrix4x4f
	{
		public float c_00;
		public float c_01;
		public float c_02;
		public float c_03;
		public float c_10;
		public float c_11;
		public float c_12;
		public float c_13;
		public float c_20;
		public float c_21;
		public float c_22;
		public float c_23;
		public float c_30;
		public float c_31;
		public float c_32;
		public float c_33;
	};

	[System.Serializable]
	[StructLayout(LayoutKind.Sequential)]
	public struct RectStruct
	{
		public float x; //in cm
		public float y; //in cm
		public float width;
		public float height;
	};
	#endregion

	#region Tracker structures 

	public enum StateTracker
	{
		Lost = -1,
		Undefined = 0,
		Live,
		Occluded,
		OutOfCamera
	};

	[System.Serializable]
	[StructLayout(LayoutKind.Sequential)]
	public struct Target
	{
		public short id;
		public RectStruct rect;
		public StateTracker state;
	};

	#endregion

	#region Frame structures

	public enum FlipMode
	{
		None = 0,
		FlipBoth,
		FlipY,
		FlipX,
	};

	[System.Serializable]
	[StructLayout(LayoutKind.Sequential)]
	public struct Frame
	{
		public int width;
		public int height;
		public FlipMode flip_mode;
		public IntPtr raw_data;
	};

	#endregion

	/* Wrapper */

	[PluginAttr("cards_rgbtrack")]
	public static class CARDSTrackingPlugin
	{
#if UNITY_EDITOR
		[PluginFunctionAttr("Init")]
		public static Init init = null;
		public unsafe delegate void Init(Target* targets, ref int nbTarget, int maxTarget);

		[PluginFunctionAttr("Close")]
		public static Close close = null;
		public unsafe delegate void Close(Target* targets, ref int nbTarget, int maxTarget);

		[PluginFunctionAttr("Register")]
		public static Register register = null;
		public unsafe delegate bool Register(int id, ref Frame frame, ref RectStruct zoneObject, Target* targets, ref int nbTarget, int maxTarget);

		[PluginFunctionAttr("UnRegister")]
		public static UnRegister un_register = null;
		public unsafe delegate void UnRegister(int id, Target* targets, ref int nbTarget);

		[PluginFunctionAttr("Detect")]
		public static Detect detect = null;
		public unsafe delegate void Detect(ref Frame frame, ref RectStruct zoneDetection, Target* targets, ref int nbTarget, int maxTarget);

		[PluginFunctionAttr("CheckTrack")]
		public static CheckTrack check_track = null;
		public unsafe delegate void CheckTrack(ref Frame frame, Target* targets, int nbTarget);

		[PluginFunctionAttr("Track")]
		public static Track track = null;
		public unsafe delegate void Track(ref Frame frame, Target* targets, int nbTarget);
#else
        [DllImport("cards_rgbtrack")]
		internal unsafe static extern void Init(Target* targets, ref int nbTarget, int maxTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void Close(Target* targets, ref int nbTarget, int maxTarget);
		
		[DllImport("cards_rgbtrack")]
		internal unsafe static extern bool Register(int id, ref Frame frame, ref RectStruct zoneObject, Target* targets, ref int nbTarget, int maxTarget);
		 
		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void UnRegister(int id, Target* targets, ref int nbTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void Detect(ref Frame frame, ref RectStruct zoneDetection, Target* targets, ref int nbTarget, int maxTarget);
		
		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void CheckTrack(ref Frame frame, Target* targets, int nbTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void Track(ref Frame frame, Target* targets, int nbTarget);
#endif

		// The plugin methods are wrapped in order to be transparent for the users when they are being executed in 
		// editor or standalone mode.
		#region Wrapped methods

		public static unsafe void InitWrapped(Target* targets, ref int nbTarget, int maxTarget) =>
#if UNITY_EDITOR
			CARDSTrackingPlugin.init(targets, ref nbTarget, maxTarget);
#else
            Init(targets, ref nbTarget, maxTarget);
#endif


		public static unsafe void CloseWrapped(Target* targets, ref int nbTarget, int maxTarget) =>
#if UNITY_EDITOR
			CARDSTrackingPlugin.close(targets, ref nbTarget, maxTarget);
#else
            Close(targets, ref nbTarget, maxTarget);
#endif

		public static unsafe void RegisterWrapped(int id, ref Frame frame, ref RectStruct zoneObject, Target* targets, ref int nbTarget, int maxTarget) =>
#if UNITY_EDITOR
			CARDSTrackingPlugin.register(id, ref frame, ref zoneObject, targets, ref nbTarget, maxTarget);
#else
            Register(id, ref frame, ref zoneObject, targets, ref nbTarget, maxTarget);
#endif

		public static unsafe void UnRegisterWrapped(int id, Target* targets, ref int nbTarget) =>
#if UNITY_EDITOR
			CARDSTrackingPlugin.un_register(id, targets, ref nbTarget);
#else
            UnRegister(id, targets, ref nbTarget);
#endif


		public static unsafe void DetectWrapped(ref Frame frame, ref RectStruct zoneDetection, Target* targets, ref int nbTarget, int maxTarget) =>
#if UNITY_EDITOR
			CARDSTrackingPlugin.detect(ref frame, ref zoneDetection, targets, ref nbTarget, maxTarget);
#else
            Detect(ref frame, ref zoneDetection, targets, ref nbTarget, maxTarget);
#endif

		public static unsafe void CheckTrackWrapped(ref Frame frame, Target* targets, int nbTarget) =>
#if UNITY_EDITOR
			CARDSTrackingPlugin.check_track(ref frame, targets, nbTarget);
#else
            CheckTrack(ref frame, targets, nbTarget);
#endif


		public static unsafe void TrackWrapped(ref Frame frame, Target* targets, int nbTarget) =>
#if UNITY_EDITOR
			CARDSTrackingPlugin.track(ref frame, targets, nbTarget);
#else
            Track(ref frame, targets, nbTarget);
#endif

		#endregion
	}
}