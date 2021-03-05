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
		Undefined = -1,
		Live,
		Occluded,
		Lost
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
		public IntPtr rawData;
	};

	[PluginAttr("cards_rgbtrack")]
	public static class SARPlugin
	{
#if UNITY_EDITOR
		[PluginFunctionAttr("Init")]
		public static Init init = null;
		public delegate void Init();

		[PluginFunctionAttr("Close")]
		public static Close close = null;
		public delegate void Close();

		[PluginFunctionAttr("Detect")]
		public static Detect detect = null;
		public unsafe delegate void Detect(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget);

		[PluginFunctionAttr("Track")]
		public static Track track = null;
		public unsafe delegate void Track(ref Frame frame, Target* targets, int nbTarget);

		[PluginFunctionAttr("DebugTargets")]
		public static DebugTargets debugTargets = null;
		public unsafe delegate void DebugTargets(ref Frame frame, Target* targets, int nbTarget);
#else
        [DllImport("cards_rgbtrack")]
		internal static extern void Init(ref int size);

		[DllImport("cards_rgbtrack")]
		internal static extern void Close();

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void Detect(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void Track(ref Frame frame, Target* targets, int nbTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void DebugTargets(ref Frame frame, Target* targets, int nbTarget);
#endif

		// The plugin methods are wrapped in order to be transparent for the users when they are being executed in 
		// editor or standalone mode.
		#region Wrapped methods

		public static void InitWrapped()
		{
#if UNITY_EDITOR
			SARPlugin.init();
#else
            Init();
#endif
		}

		public static void CloseWrapped()
		{
#if UNITY_EDITOR
			SARPlugin.close();
#else
            Close();
#endif
		}

		public unsafe static void DetectWrapped(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			SARPlugin.detect(ref frame, targets, ref nbTarget, maxTarget);
#else
            Detect(ref frame, targets, ref nbTarget, maxTarget);
#endif
		}

		public unsafe static void TrackWrapped(ref Frame frame, Target* targets, int nbTarget)
		{
#if UNITY_EDITOR
			SARPlugin.track(ref frame, targets, nbTarget);
#else
            Track(frame, targets, nbTarget);
#endif
		}

		public unsafe static void DebugTargetsWrapped(ref Frame frame, Target* targets, int nbTarget)
		{
#if UNITY_EDITOR
			SARPlugin.debugTargets(ref frame, targets, nbTarget);
#else
            DebugTargets(frame, targets, nbTarget);
#endif
		}

		#endregion
	}
}