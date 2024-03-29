﻿using System.Collections.Generic;
using System.IO;

using Plugin;

using UnityEngine;

/// <summary>
/// The parent component for main tracking objects loop. 
/// Open FrameProvider, handle memory and getters, logs activation, and assure the compatibility of space between OpenCV and Unity. 
/// </summary>
public abstract class Tracking : MonoBehaviour
{
	[SerializeField]
	protected VideoParameters parameters; //Scenario that you can interchange (Virtual, Webcam)

	protected Frame frame_buffer;
	protected Frame frame_buffer_background;
	protected RectStruct zone_detection;
	protected FrameProvider video;
	protected int nb_targets = 0, max_targets = 5;
	protected Target[] targets;
	protected int nb_frame = -1;

	#region Getters properties
	public bool IsInitialised
	{
		get
		{
			return nb_frame > parameters.starting_frame;
		}
	}

	public VideoParameters Parameters
	{
		get
		{
			return parameters;
		}
	}

	public int NbTargets
	{
		get
		{
			return nb_targets;
		}
	}

	public int MaxTargets
	{
		get
		{
			return max_targets;
		}
	}

	public int NbFrame
	{
		get
		{
			return nb_frame;
		}
	}
	#endregion

	/// <summary>
	/// Convert from OpenCV space to Unity and give a copy of targets
	/// </summary>
	/// <returns>The list of targets.</returns>
	public List<Target> GetTargets()
	{
		List<Target> final = new List<Target>();
		for(int i = 0; i < MaxTargets; i++)
		{
			Vector2f original_size = targets[i].original_size;
			original_size.y *= -1;
			Target t = new Target
			{
				id = targets[i].id,
				rect = ConvertRectToUnityScreen(targets[i].rect),
				original_size = original_size,
				state = targets[i].state
			};
			final.Add(t);
		}
		return final;
	}

	/// <summary>
	/// Convert RectStruct from opencv to Unity space.
	/// bottom left = 0,1, top right = 0,1 in OpenCV
	/// bottom left = 0,0, top right = 1,1 in Unity
	/// </summary>
	/// <param name="rect">The rect to convert space.</param>
	/// <returns>The new rect.</returns>
	protected RectStruct ConvertRectToUnityScreen(RectStruct rect)
	{
		rect.y = parameters.camera_height - rect.y;
		rect.height = -rect.height;
		return rect;
	}

	protected RectStruct ConvertUnityScreenToRect(Rect rect)
	{
		RectStruct rect_s;
		Vector2 pt1 = parameters.rect_detection.min;
		Vector2 pt2 = parameters.rect_detection.max;

		rect_s.x = pt1.x;
		rect_s.y = parameters.camera_height - pt2.y;
		rect_s.width = parameters.rect_detection.width;
		rect_s.height = parameters.rect_detection.height;
		return rect_s;
	}

	protected void Awake()
	{
		targets = new Target[5];
		Application.targetFrameRate = parameters.requested_camera_fps;
		LogsData.Instance.enabled = parameters.log_datas;
	}

	/// <summary>
	/// Start point.
	/// </summary>
	protected void OnEnable()
	{
		DebugTargets[] debugs = gameObject.GetComponents<DebugTargets>();
		if(debugs.Length > 0)
		{
			Debug.LogError("Debug create dynamically, component will be removed.");
			foreach(DebugTargets debug in debugs)
			{
				DestroyImmediate(debug);
			}
		}

		Debug.Log("Start tracking.");

		//File calibration
		if(parameters.use_file_calibration)
		{
			string path = Path.Combine(Application.streamingAssetsPath, parameters.path_subfolder_calibration);
			if(!Directory.Exists(path))
			{
				throw new System.Exception("Folder " + path + " does not exist.");
			}
			if(!CARDSCalibrationPlugin.GetPoseParametersWrapped(path, ref parameters.calibration))
			{
				throw new System.Exception("Loading parameters " + path + " failed.");
			}
		}
		else if(parameters.device_index != -1)
		{
			Debug.LogWarning("Not recommended to not use calibrtation file in other context than virtual.");
		}

		// Init video
		if(parameters.UseWebcam)
		{
			video = new WebcamTexture();
			gameObject.AddComponent<DebugTargetsWebcam>();
		}
		else if(parameters.device_index == -1)
		{
			video = new VirtualCameraTexture();
			gameObject.AddComponent<DebugTargetsVirtual>();
		}
		else if(parameters.device_index == -2)
		{
			video = new VideoFileTexture();
			gameObject.AddComponent<DebugTargetsVideo>();
		}
		video.Init(parameters);

		// Init plugin
		unsafe
		{
			fixed(Target* outTargets = targets)
			{
				CARDSTrackingPlugin.InitWrapped(outTargets, ref nb_targets, max_targets);
			}
		}

		//Init detection
		if(parameters.use_detection)
		{
			zone_detection = ConvertUnityScreenToRect(parameters.rect_detection);
		}

		nb_frame = -1;
	}

	/// <summary>
	/// End point.
	/// </summary>
	protected void OnApplicationQuit()
	{
		FreeInternData();
		nb_frame = -1;
	}

	/// <summary>
	/// End point, done after OnApplicationQuit.
	/// Not done in Build when quitting application.
	/// </summary>
	protected void OnDisable()
	{
		FreeInternData();
		nb_frame = -1;
	}

	private void FreeInternData()
	{
		video.Close();
		if(nb_frame > parameters.starting_frame)
		{
			if(parameters.use_detection == true)
			{
				CARDSVideoPlugin.FreeCopyFrameWrapped(ref frame_buffer_background);
			}
			unsafe
			{
				fixed(Target* out_targets = targets)
				{
					Debug.Log("Free targets");
					CARDSTrackingPlugin.CloseWrapped(out_targets, ref nb_targets, max_targets);
				}
			}
		}
	}

	protected void Update()
	{
		//Escape to quit.
		if(Input.GetKeyDown(KeyCode.Escape))
		{
			gameObject.SetActive(false);

#if UNITY_EDITOR
			UnityEditor.EditorApplication.isPlaying = false;
#elif UNITY_WEBPLAYER
         Application.OpenURL(webplayerQuitURL);
#else
         Application.Quit();
#endif
			return;
		}

		//Main loop
		if(parameters.log_datas)
		{
			LogsData.Instance.DebugFramePerf(InternalTrackingLoop);
		}
		else
		{
			InternalTrackingLoop();
		}
	}

	protected abstract void InternalTrackingLoop();
}
