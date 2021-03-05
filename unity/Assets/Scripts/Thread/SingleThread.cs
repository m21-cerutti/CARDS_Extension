using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Plugin;

public class SingleThread : MonoBehaviour
{
	[SerializeField]
	private int _camera_width = 256;
	[SerializeField]
	private int _camera_height = 256;
	[SerializeField]
	private int _requested_camera_fps = 60;

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
		Application.targetFrameRate = _requested_camera_fps;
		_is_initialised = false;
		_nb_frame = -1;
		_nb_targets = 0;
	}

	void Update()
	{

		if(!_is_initialised && Input.GetKeyDown(KeyCode.Escape))
		{
			Debug.Log("Start tracking.");
			//TODO Injection dependency to do
			//_video = new WebcamTexture();
			_video = new VirtualCameraTexture();
			_video.Init(_camera_width, _camera_height, _requested_camera_fps);
			_is_initialised = true;
		}
		else if(_is_initialised)
		{
			Frame fr = _video.GetFrame();

			if(_nb_frame < 0)
				SARPlugin.InitWrapped();
			else if(_nb_frame < 1)
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
			else if(_nb_frame > 0)
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
