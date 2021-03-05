using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Plugin;

public class SingleThread : MonoBehaviour
{
	[SerializeField]
	private VideoParameters _parameters;
	[SerializeField]
	private bool _use_webcam = false;
	private VideoProvider _video;

	private int _nb_targets = 0, _max_targets = 5;
	private Target[] _targets;

	private bool _is_initialised = false;
	private int _nb_frame = -1;

	//Debug
	[SerializeField]
	private GameObject[] _game_targets;

	private void Awake()
	{
		_targets = new Target[5];
		Application.targetFrameRate = _parameters.requested_camera_fps;
		_is_initialised = false;
		_nb_frame = -1;
		_nb_targets = 0;
	}

	void Update()
	{
		if(!_is_initialised && Input.GetKeyDown(KeyCode.Escape))
		{
			Debug.Log("Start tracking.");
			if(_use_webcam)
			{
				_video = new WebcamTexture();
			}
			else
			{
				_video = new VirtualCameraTexture();
			}
			_video.Init(_parameters.camera_width, _parameters.camera_height, _parameters.requested_camera_fps);
			_is_initialised = true;
		}
		else if(_is_initialised && _nb_frame < 0)
		{
			SARPlugin.InitWrapped();
			_nb_frame = 0;
		}
		else if(_is_initialised)
		{
			Frame fr;
			if(!_video.GetFrame(out fr))
			{
				return;
			}

			if(_nb_frame == _parameters.starting_frame)
			{
				unsafe
				{
					fixed(Target* outTargets = _targets)
					{
						Debug.Log("Detect");
						SARPlugin.DetectWrapped(ref fr, outTargets, ref _nb_targets, _max_targets);
					}
				}
			}
			else if(_nb_frame > _parameters.starting_frame)
			{
				unsafe
				{
					fixed(Target* outTargets = _targets)
					{
						Debug.Log("Track");
						SARPlugin.TrackWrapped(ref fr, outTargets, _nb_targets);
						SARPlugin.DebugTargetsWrapped(ref fr, outTargets, _nb_targets);
					}
				}
			}
			_nb_frame++;
		}
	}
}
