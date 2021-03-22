using Plugin;

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
					parameters.use_detection = true;
				}
			}
		}
		else if(nb_frame > parameters.starting_frame)
		{
			//DETECTION
			if(parameters.use_detection)
			{
				parameters.rect_detection.x = (float)(frame_buffer.width * 0.05);
				parameters.rect_detection.y = (float)(frame_buffer.height * 0.55);
				parameters.rect_detection.width = (float)(frame_buffer.width * 0.5 - frame_buffer.width * 0.1);
				parameters.rect_detection.height = (float)(frame_buffer.height * 0.9 - frame_buffer.height * 0.5);

				RectStruct zone_detection;
				zone_detection.x = (float)(frame_buffer.width * 0.05);
				zone_detection.y = (float)(frame_buffer.height * 0.55);
				zone_detection.width = (float)(frame_buffer.width * 0.5 - frame_buffer.width * 0.1);
				zone_detection.height = (float)(frame_buffer.height * 0.9 - frame_buffer.height * 0.5);
				if(nb_frame == parameters.saving_background)
                {
					frame_buffer_background = video.GetCopyFrame();
                }
				if ((nb_frame % parameters.detection_frequency) == 0)
				{
					unsafe
					{
						fixed (Target* outTargets = targets)
						{
							//Debug.Log("Detect");
							CARDSTrackingPlugin.DetectWrapped(ref frame_buffer, ref frame_buffer_background, ref zone_detection, outTargets, ref nb_targets, max_targets);
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
