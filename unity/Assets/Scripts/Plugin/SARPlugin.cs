using fts;

using System;
using System.Runtime.InteropServices;

using UnityEngine;

namespace Plugin
{
	[System.Serializable]
	public struct Vector3f
	{
		public float x;
		public float y;
		public float z;
	};

	[System.Serializable]
	public struct Matrix4x4
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

	/*Tacker definition*/
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
		public short ID;
		public RectStruct rect;
		public StateTracker state;
	};

	//TODO Structure export ?

	/*Use for frame pass from C# to C++*/

	[System.Serializable]
	[StructLayout(LayoutKind.Sequential)]
	public struct Frame
	{
		public int width;
		public int height;
		public bool is_flipped;
		public IntPtr raw_data;
	};

	[PluginAttr("cards_rgbtrack")]
	public static class SARPlugin
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

		[PluginFunctionAttr("ManualRegister")]
		public static ManualRegister manual_register = null;
		public unsafe delegate void ManualRegister(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget);

		[PluginFunctionAttr("DebugTargets")]
		public static DebugTargets debug_targets = null;
		public unsafe delegate void DebugTargets(ref Frame frame, Target* targets, int nbTarget);
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
		
		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void ManualRegister(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void DebugTargets(ref Frame frame, Target* targets, int nbTarget);
#endif

		// The plugin methods are wrapped in order to be transparent for the users when they are being executed in 
		// editor or standalone mode.
		#region Wrapped methods

		public unsafe static void InitWrapped(Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			SARPlugin.init(targets, ref nbTarget, maxTarget);
#else
            Init(targets, ref nbTarget, maxTarget);
#endif
		}

		public unsafe static void CloseWrapped(Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			SARPlugin.close(targets, ref nbTarget, maxTarget);
#else
            Close(targets, ref nbTarget, maxTarget);
#endif
		}
		public unsafe static void RegisterWrapped(int id, ref Frame frame, ref RectStruct zoneObject, Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			SARPlugin.register(id, ref frame, ref zoneObject, targets, ref nbTarget, maxTarget);
#else
            Register(id, ref frame, ref zoneObject, targets, ref nbTarget, maxTarget);
#endif
		}
		public unsafe static void UnRegisterWrapped(int id, Target* targets, ref int nbTarget)
		{
#if UNITY_EDITOR
			SARPlugin.un_register(id, targets, ref nbTarget);
#else
            UnRegister(id, targets, ref nbTarget);
#endif
		}

		public unsafe static void DetectWrapped(ref Frame frame, ref RectStruct zoneDetection, Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			SARPlugin.detect(ref frame, ref zoneDetection, targets, ref nbTarget, maxTarget);
#else
            Detect(ref frame, ref zoneDetection, targets, ref nbTarget, maxTarget);
#endif
		}
		public unsafe static void CheckTrackWrapped(ref Frame frame, Target* targets, int nbTarget)
		{
#if UNITY_EDITOR
			SARPlugin.check_track(ref frame, targets, nbTarget);
#else
            CheckTrack(ref frame, targets, nbTarget);
#endif
		}

		public unsafe static void TrackWrapped(ref Frame frame, Target* targets, int nbTarget)
		{
#if UNITY_EDITOR
			SARPlugin.track(ref frame, targets, nbTarget);
#else
            Track(ref frame, targets, nbTarget);
#endif
		}

		public unsafe static void ManualRegisterWrapped(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			SARPlugin.manual_register(ref frame, targets, ref nbTarget, maxTarget);
#else
            ManualRegister(ref frame, targets, ref nbTarget, maxTarget);
#endif
		}

		public unsafe static void DebugTargetsWrapped(ref Frame frame, Target* targets, int nbTarget)
		{
#if UNITY_EDITOR
			SARPlugin.debug_targets(ref frame, targets, nbTarget);
#else
            DebugTargets(ref frame, targets, nbTarget);
#endif
		}

		#endregion
	}
}