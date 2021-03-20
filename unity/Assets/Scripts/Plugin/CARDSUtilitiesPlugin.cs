using System.Runtime.InteropServices;

using fts;

namespace Plugin
{
	/// <summary>
	/// Plugin wrapper functions for utilities, like debugging.
	/// </summary>
	[PluginAttr("cards_rgbtrack")]
	public static class CARDSUtilitiesPlugin
	{
#if UNITY_EDITOR
		[PluginFunctionAttr("ManualRegister")]
		public static ManualRegister manual_register = null;
		public unsafe delegate void ManualRegister(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget);

		[PluginFunctionAttr("DebugTargets")]
		public static DebugTargets debug_targets = null;
		public unsafe delegate void DebugTargets(ref Frame frame, Target* targets, int maxTarget);
#else
		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void ManualRegister(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget);

		[DllImport("cards_rgbtrack")]
		internal unsafe static extern void DebugTargets(ref Frame frame, Target* targets, int nbTarget);
#endif

		// The plugin methods are wrapped in order to be transparent for the users when they are being executed in 
		// editor or standalone mode.
		#region Wrapped methods
		public static unsafe void ManualRegisterWrapped(ref Frame frame, Target* targets, ref int nbTarget, int maxTarget)
		{
#if UNITY_EDITOR
			manual_register(ref frame, targets, ref nbTarget, maxTarget);
#else
            ManualRegister(ref frame, targets, ref nbTarget, maxTarget);
#endif
		}

		public static unsafe void DebugTargetsWrapped(ref Frame frame, Target* targets, int maxTarget)
		{
#if UNITY_EDITOR
			debug_targets(ref frame, targets, maxTarget);
#else
            DebugTargets(ref frame, targets, maxTarget);
#endif
		}


		#endregion
	}
}