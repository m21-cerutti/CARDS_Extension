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
				zone_detection.x = parameters.rect_detection.x;
				zone_detection.y = parameters.rect_detection.y;
				zone_detection.width = parameters.rect_detection.width;
				zone_detection.height = parameters.rect_detection.height;
				if(nb_frame == parameters.saving_background)
                {
					frame_buffer_background = CARDSVideoPlugin.GetCopyFrameWrapped(ref frame_buffer);
					Debug.LogError(frame_buffer_background.raw_data != IntPtr.Zero);
					
				}
				if (((nb_frame % parameters.detection_frequency) == 0) && (frame_buffer_background.raw_data != null))
				{
					unsafe
					{
						fixed (Target* outTargets = targets)
						{
							//Debug.Log("Detect");
							CARDSTrackingPlugin.DetectWrapped(ref frame_buffer, ref frame_buffer_background, ref zone_detection, outTargets, ref nb_targets, max_targets);
							if (parameters.debug_cv)
							{
								CARDSUtilitiesPlugin.DebugTargetsWrapped(ref frame_buffer, outTargets, max_targets);
							}
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
						if (parameters.debug_cv)
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
