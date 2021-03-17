﻿using Plugin;

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
				//TODO convert rect_detection
				unsafe
				{
					fixed(Target* outTargets = targets)
					{
						//Debug.Log("Detect");
						//SARPlugin.DetectWrapped(ref fr, _parameters.rect_detection, outTargets, ref _nb_targets, _max_targets);
					}
				}
			}

			//CHECK TRACK
			if((nb_frame % parameters.checktrack_frequency) == 0)
			{
				/*
				unsafe
				{
					fixed(Target* outTargets = _targets)
					{
						//Debug.Log("CheckTrack");
						//TODO
						//SARPlugin.CheckTrackWrapped(ref fr, outTargets, _nb_targets);
						SARPlugin.DebugTargetsWrapped(ref fr, outTargets, _nb_targets);
					}
				}

				if(!_use_webcam)
					CompareGameObjectVirtual();
				*/
			}
			else // TRACK
			{
				unsafe
				{
					fixed(Target* outTargets = targets)
					{
						//Debug.Log("Track");
						CARDSTrackingPlugin.TrackWrapped(ref frame_buffer, outTargets, nb_targets);
						if(parameters.debug_cv)
						{
							CARDSUtilitiesPlugin.DebugTargetsWrapped(ref frame_buffer, outTargets, nb_targets);
						}
					}
				}
			}
		}
		nb_frame++;
	}
}
