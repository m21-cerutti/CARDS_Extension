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
	private List<GameObject> _game_targets;

	private void Awake()
	{
		_targets = new Target[5];
		Application.targetFrameRate = _parameters.requested_camera_fps;
		_is_initialised = false;
		_nb_frame = -1;
		_nb_targets = 0;
	}
	private void Start()
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

	void Update()
	{
		if(_is_initialised && _nb_frame < 0)
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
						//Debug.Log("Detect");
						SARPlugin.DetectWrapped(ref fr, outTargets, ref _nb_targets, _max_targets);
					}
				}

				if(!_use_webcam)
					RegisterGameObjectVirtual();

			}
			else if(_nb_frame > _parameters.starting_frame)
			{
				unsafe
				{
					fixed(Target* outTargets = _targets)
					{
						//Debug.Log("Track");
						SARPlugin.TrackWrapped(ref fr, outTargets, _nb_targets);
						SARPlugin.DebugTargetsWrapped(ref fr, outTargets, _nb_targets);
					}
				}
				//Debug.Log("Total Tracked : " + _nb_targets);
				if(!_use_webcam)
					CompareGameObjectVirtual();
			}
			_nb_frame++;
		}
	}

	private void RegisterGameObjectVirtual()
	{
		//Check object detected
		for(int i = 0; i < _nb_targets; i++)
		{
			Target t = _targets[i];
			Vector2 point = GetCenterScreenTarget(t);
			Ray ray = Camera.main.ScreenPointToRay(point);
			RaycastHit hit;
			if(Physics.Raycast(ray, out hit, 5.0f))
			{
				_game_targets.Add(hit.collider.gameObject);
			}
		}
	}

	private void CompareGameObjectVirtual()
	{
		float global_error = 0;
		for(int i = 0; i < _nb_targets; i++)
		{
			Target t = _targets[i];
			Vector3 real_position = _game_targets[i].transform.position;

			Vector3 point = GetCenterScreenTarget(t);
			Vector3 prevision = Camera.main.ScreenToWorldPoint(point);

			//TODO Use z but need pose
			prevision.y = real_position.y;
			//Debug.Log("Target " + i + " : " + point);

			float error = Vector3.Distance(real_position, prevision);
			//Debug.Log("Target " + i + " : " + error);
			global_error += error;
		}
		global_error /= _nb_targets;
		Debug.Log("Global error : " + global_error);
	}

	private Vector3 GetCenterScreenTarget(Target t)
	{
		return new Vector3(t.rect.x + ((float)(t.rect.width) / 2.0f), t.rect.y + ((float)(t.rect.height) / 2.0f), Camera.main.transform.position.magnitude);
	}

#if UNITY_EDITOR
	void OnDrawGizmos()
	{
		for(int i = 0; i < _nb_targets && !_use_webcam; i++)
		{
			Target t = _targets[i];
			Vector3 real_position = _game_targets[i].transform.position;
			Vector3 point = GetCenterScreenTarget(t);
			Vector3 prevision = Camera.main.ScreenToWorldPoint(point);
			prevision.y = real_position.y;
			Gizmos.color = Color.yellow;
			Gizmos.DrawCube(prevision, 0.1f * Vector3.one);
		}
	}
#endif
}
