
using Plugin;

using UnityEngine;

/// <summary>
/// See DebugTargets.
/// Nothing to compare with webcam.
/// </summary>
public class DebugTargetsWebcam : DebugTargets
{
	// Do nothing, webcam not using precise position, use for debug.
	protected override bool GetRealPositionTarget(int id, out Vector3 pos, out StateTracker state)
	{
		pos = Vector3.zero;
		state = StateTracker.Live;
		return true;
	}

	protected override void Start()
	{
		base.Start();
		if(!parameters.UseWebcam)
		{
			throw new System.Exception("Never use a virtual camera with this script !");
		}
	}
}
