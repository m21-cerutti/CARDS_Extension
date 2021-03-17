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
	public struct Vector2f
	{
		public float x;
		public float y;
	};


	[System.Serializable]
	public struct Vector3f
	{
		public float x;
		public float y;
		public float z;
	};

	[System.Serializable]
	public struct Matrix3x3f
	{
		public float c_00;
		public float c_01;
		public float c_02;
		public float c_10;
		public float c_11;
		public float c_12;
		public float c_20;
		public float c_21;
		public float c_22;
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
		public float x;
		public float y;
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
		public Vector2f original_size;
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
		public unsafe delegate int Register(ref Frame frame, ref RectStruct zone, Target* targets, ref int nbTarget, int maxTarget);

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

		[PluginFunctionAttr("EstimatePose")]
		public static EstimatePose estimate_pose = null;
		public delegate Matrix4x4f EstimatePose(ref Target target, Matrix3x3f intrinsic, float dist_cam_table);
#else
        [DllImport("cards_rgbtrack")]
		internal unsafe static extern void Init(Target* targets, ref int nbTarget, int maxTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void Close(Target* targets, ref int nbTarget, int maxTarget);
		
		[DllImport("cards_rgbtrack")]
		internal unsafe static extern int Register(ref Frame frame, ref RectStruct zone, Target* targets, ref int nbTarget, int maxTarget);
		 
		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void UnRegister(int id, Target* targets, ref int nbTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void Detect(ref Frame frame, ref RectStruct zoneDetection, Target* targets, ref int nbTarget, int maxTarget);
		
		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void CheckTrack(ref Frame frame, Target* targets, int nbTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void Track(ref Frame frame, Target* targets, int nbTarget);

		[DllImport("cards_rgbtrack")]
		internal static extern Matrix4x4f EstimatePose( ref Target target, Matrix3x3f intrinsic);
#endif

		// The plugin methods are wrapped in order to be transparent for the users when they are being executed in 
		// editor or standalone mode.
		#region Wrapped methods

		public static unsafe void InitWrapped(Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			CARDSTrackingPlugin.init(targets, ref nbTarget, maxTarget);
#else
            Init(targets, ref nbTarget, maxTarget);
#endif
		}


		public static unsafe void CloseWrapped(Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			CARDSTrackingPlugin.close(targets, ref nbTarget, maxTarget);
#else
            Close(targets, ref nbTarget, maxTarget);
#endif
		}

		public static unsafe void RegisterWrapped(ref Frame frame, ref RectStruct zone, Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			CARDSTrackingPlugin.register(ref frame, ref zone, targets, ref nbTarget, maxTarget);
#else
            Register(ref frame, ref zone, targets, ref nbTarget, maxTarget);
#endif
		}

		public static unsafe void UnRegisterWrapped(int id, Target* targets, ref int nbTarget)
		{
#if UNITY_EDITOR
			CARDSTrackingPlugin.un_register(id, targets, ref nbTarget);
#else
            UnRegister(id, targets, ref nbTarget);
#endif
		}

		public static unsafe void DetectWrapped(ref Frame frame, ref RectStruct zoneDetection, Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			CARDSTrackingPlugin.detect(ref frame, ref zoneDetection, targets, ref nbTarget, maxTarget);
#else
            Detect(ref frame, ref zoneDetection, targets, ref nbTarget, maxTarget);
#endif
		}

		public static unsafe void CheckTrackWrapped(ref Frame frame, Target* targets, int nbTarget)
		{
#if UNITY_EDITOR
			CARDSTrackingPlugin.check_track(ref frame, targets, nbTarget);
#else
            CheckTrack(ref frame, targets, nbTarget);
#endif
		}

		public static unsafe void TrackWrapped(ref Frame frame, Target* targets, int nbTarget)
		{
#if UNITY_EDITOR
			CARDSTrackingPlugin.track(ref frame, targets, nbTarget);
#else
            Track(ref frame, targets, nbTarget);
#endif
		}

		public static Matrix4x4f EstimatePoseWrapped(ref Target target, Matrix3x3f intrinsic, float dist_cam_table)
		{
#if UNITY_EDITOR
			return CARDSTrackingPlugin.estimate_pose(ref target, intrinsic, dist_cam_table);
#else
            return EstimatePose(ref target, intrinsic, dist_cam_table);
#endif
		}

		#endregion
	}
}