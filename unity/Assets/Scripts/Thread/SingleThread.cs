using Plugin;

using System;

using UnityEngine;

public class SingleThread : Tracking
{
	/// <summary>
	/// Implementation in one thread.
	/// </summary>
	protected override void InternalTrackingLoop()
	{
		if(!video.GetFrame(out frame_buffer))
		{
			Debug.LogWarning("Empty frames.");
			return;
		}

		//Bakground
		if(parameters.use_detection && nb_frame == parameters.saving_background)
		{
			Debug.Log("Register background");
			CARDSVideoPlugin.GetCopyFrameWrapped(ref frame_buffer, ref frame_buffer_background);
		}

		if(nb_frame == parameters.starting_frame)
		{
			unsafe
			{
				fixed(Target* outTargets = targets)
				{
					//Debug.Log("Register manual");
					Time.timeScale = 0;
					CARDSUtilitiesPlugin.ManualRegisterWrapped(ref frame_buffer, outTargets, ref nb_targets, max_targets);
					Time.timeScale = 1;
				}
			}
		}
		else if(nb_frame > parameters.starting_frame)
		{
			//DETECTION
			if(parameters.use_detection)
			{
				if(((nb_frame % parameters.detection_frequency) == 0) && (frame_buffer_background.raw_data != null))
				{
					unsafe
					{
						fixed(Target* out_targets = targets)
						{
							Debug.Log("Detect");
							CARDSTrackingPlugin.DetectWrapped(ref frame_buffer, ref frame_buffer_background, ref zone_detection, out_targets, ref nb_targets, max_targets);
						}
					}
				}
			}

			//CHECK TRACK
			if((nb_frame % parameters.checktrack_frequency) == 0)
			{
				unsafe
				{
					fixed(Target* outTargets = targets)
					{
						//Debug.Log("CheckTrack");
						CARDSTrackingPlugin.CheckTrackWrapped(ref frame_buffer, outTargets, max_targets);
						if(parameters.debug_cv)
						{
							CARDSUtilitiesPlugin.DebugTargetsWrapped(ref frame_buffer, outTargets, max_targets);
						}
					}
				}
			}
			else // TRACK
			{
				unsafe
				{
					fixed(Target* outTargets = targets)
					{
						//Debug.Log("Track");
						CARDSTrackingPlugin.TrackWrapped(ref frame_buffer, outTargets, max_targets);
						if(parameters.debug_cv)
						{
							CARDSUtilitiesPlugin.DebugTargetsWrapped(ref frame_buffer, outTargets, max_targets);
						}
					}
				}
			}
		}
		nb_frame++;
	}
}
