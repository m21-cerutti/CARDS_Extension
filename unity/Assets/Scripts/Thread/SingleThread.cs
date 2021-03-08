using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Plugin;

public class SingleThread : MonoBehaviour
{
	[SerializeField]
	private VideoParameters _parameters;

	private bool _use_webcam = false;
	private VideoProvider _video;

	private int _nb_targets = 0, _max_targets = 5;
	private Target[] _targets;

	private int _nb_frame = -1;

	//Debug
	private List<GameObject> _game_targets = new List<GameObject>();

	private void Awake()
	{
		_targets = new Target[5];
		Application.targetFrameRate = _parameters.requested_camera_fps;
		_nb_frame = -1;
		_nb_targets = 0;
		_use_webcam = _parameters.device_index >= 0;
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
		_video.Init(_parameters);
		SARPlugin.InitWrapped();
		_nb_frame = -1;
	}

	void Update()
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
					SARPlugin.ManualRegisterWrapped(ref fr, outTargets, ref _nb_targets, _max_targets);
				}
			}
			Debug.Log("Total Tracked : " + _nb_targets);
			if(!_use_webcam)
				RegisterGameObjectVirtual();
		}
		else if(_nb_frame > _parameters.starting_frame)
		{
			//DETECTION
			//TODO convert rect_detection
			unsafe
			{
				fixed(Target* outTargets = _targets)
				{
					//Debug.Log("Detect");
					//SARPlugin.DetectWrapped(ref fr, _parameters.rect_detection, outTargets, ref _nb_targets, _max_targets);
				}
			}

			//CHECK TRACK
			if((_nb_frame % _parameters.detect_frequency) == 0)
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
					fixed(Target* outTargets = _targets)
					{
						//Debug.Log("Track");
						SARPlugin.TrackWrapped(ref fr, outTargets, _nb_targets);
						SARPlugin.DebugTargetsWrapped(ref fr, outTargets, _nb_targets);
					}
				}
				if(!_use_webcam)
					CompareGameObjectVirtual();
			}
		}
		_nb_frame++;
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
	/*
	private void OnGUI()
	{
		GUI.color = Color.red;
		GUI.Box(_parameters.rect_detection, Texture2D.whiteTexture);
		GUI.color = Color.white;
	}
	*/

	void OnDrawGizmos()
	{

		for(int i = 0; i < _nb_targets && !_use_webcam; i++)
		{
			Target t = _targets[i];
			Vector3 real_position = _game_targets[i].transform.position;
			Vector3 point = GetCenterScreenTarget(t);
			Vector3 prevision = Camera.main.ScreenToWorldPoint(point);
			prevision.y = real_position.y;
			Gizmos.color = Color.red;
			Gizmos.DrawWireSphere(prevision, 0.1f);
		}
	}
#endif
}
