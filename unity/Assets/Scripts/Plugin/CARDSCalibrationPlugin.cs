using System.Runtime.InteropServices;

using fts;

namespace Plugin
{
	/// <summary>
	/// Plugin wrapper functions for calibration.
	/// </summary>
	[PluginAttr("cards_rgbtrack")]
	public static class CARDSCalibrationPlugin
	{
#if UNITY_EDITOR
		[PluginFunctionAttr("GetPoseParameters")]
		public static GetPoseParameters get_pose_parameters = null;
		public delegate bool GetPoseParameters(string resourceDirectory, ref PoseParameters parameters);
#else
		[DllImport("cards_rgbtrack")]
		internal static extern bool GetPoseParameters(string resourceDirectory, ref PoseParameters parameters);
#endif

		// The plugin methods are wrapped in order to be transparent for the users when they are being executed in 
		// editor or standalone mode.
		#region Wrapped methods

		public static bool GetPoseParametersWrapped(string resourceDirectory, ref PoseParameters parameters)
		{
#if UNITY_EDITOR
			return get_pose_parameters(resourceDirectory, ref parameters);
#else
            return GetPoseParameters(resourceDirectory, ref parameters);
#endif
		}

		#endregion
	}
}