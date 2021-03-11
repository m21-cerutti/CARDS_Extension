using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using Plugin;

public abstract class Tracking : MonoBehaviour
{
	[SerializeField]
	protected VideoParameters parameters; //Scenario that you can interchange (Virtual, Webcam)

	protected VideoProvider video;
	protected int nb_targets = 0, max_targets = 5;
	protected Target[] targets;
	protected int nb_frame = -1;

	#region Getters properties
	public bool IsInitialised { get => nb_frame > parameters.starting_frame; }
	public int NbTargets { get => nb_targets; }
	public int MaxTargets { get => max_targets; }
	public VideoParameters Parameters { get => parameters; }
	#endregion

	public List<Target> GetTargets()
	{
		List<Target> final = new List<Target>();
		for(int i = 0; i < MaxTargets; i++)
		{
			Target t = new Target
			{
				id = targets[i].id,
				rect = ConvertRectToUnityScreen(targets[i].rect),
				state = targets[i].state
			};
			final.Add(t);
		}
		return final;
	}

	// bottom left = 0,1, top right = 0,1 in OpenCV
	// bottom left = 0,0, top right = 1,1 in Unity
	protected RectStruct ConvertRectToUnityScreen(RectStruct rect)
	{
		rect.y = parameters.camera_height - rect.y;
		rect.height = -rect.height;
		return rect;
	}

	protected void Awake()
	{
		targets = new Target[5];
		Application.targetFrameRate = parameters.requested_camera_fps;
	}

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
		if(parameters.UseWebcam)
		{
			video = new WebcamTexture();
			gameObject.AddComponent<DebugTargetsWebcam>();
		}
		else
		{
			video = new VirtualCameraTexture();
			gameObject.AddComponent<DebugTargetsVirtual>();
		}
		video.Init(parameters);

		Debug.Log("Init");
		unsafe
		{
			fixed(Target* outTargets = targets)
			{
				SARPlugin.InitWrapped(outTargets, ref nb_targets, max_targets);
			}
		}
		nb_frame = -1;
	}

	protected void OnApplicationQuit()
	{
		if(nb_frame > parameters.starting_frame)
		{
			unsafe
			{
				fixed(Target* outTargets = targets)
				{
					Debug.Log("Free");
					SARPlugin.CloseWrapped(outTargets, ref nb_targets, max_targets);
				}
			}
		}
		nb_frame = -1;
	}
	protected void OnApplicationDisable()
	{
		if(nb_frame > parameters.starting_frame)
		{
			unsafe
			{
				fixed(Target* outTargets = targets)
				{
					Debug.Log("Free");
					SARPlugin.CloseWrapped(outTargets, ref nb_targets, max_targets);
				}
			}
		}
		nb_frame = -1;
	}


	protected void Update()
	{
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
		LogsData.Instance.DebugFramePerf(InternalTrackingLoop);
	}

	abstract protected void InternalTrackingLoop();
}
