using System.Collections;
using System.Collections.Generic;

using UnityEngine;
using Plugin;
using System.Runtime.InteropServices;
using System;

public class VirtualCameraTexture : MonoBehaviour
{
	[SerializeField]
	RenderTexture renderTexture;

	Frame fr;
	private Color32[] pixels;

	int nbtargets = 0, maxTargets = 5;
	Target[] targets;

	private bool is_initialised;
	private int i = -1;

	// Start is called before the first frame update
	void Start()
	{
		fr.height = renderTexture.height;
		fr.width = renderTexture.width;
		targets = new Target[5];
		SARPlugin.InitWrapped();
	}

	// Update is called once per frame
	void Update()
	{
		Texture2D tex = new Texture2D(renderTexture.width, renderTexture.height, TextureFormat.RGB24, false);
		RenderTexture.active = renderTexture;
		tex.ReadPixels(new Rect(0, 0, renderTexture.width, renderTexture.height), 0, 0);
		tex.Apply();
		pixels = tex.GetPixels32();

		//Texture memory
		GCHandle pixelHandle = GCHandle.Alloc(pixels, GCHandleType.Pinned);
		fr.rawData = pixelHandle.AddrOfPinnedObject();

		if(!is_initialised)
		{
			if(Input.GetKeyDown(KeyCode.Escape))
				is_initialised = true;
		}
		else if(is_initialised && i < 0)
		{
			SARPlugin.InitWrapped();
		}
		else if(i < 1)
		{
			unsafe
			{
				fixed(Target* outTargets = targets)
				{
					Debug.Log("Detect");
					Debug.Log(fr.rawData);
					SARPlugin.DetectWrapped(ref fr, outTargets, ref nbtargets, maxTargets);
				}
			}
		}
		else if(i > 0)
		{
			unsafe
			{
				fixed(Target* outTargets = targets)
				{
					Debug.Log("Track");
					SARPlugin.TrackWrapped(ref fr, outTargets, nbtargets);
				}
			}
		}

		if(is_initialised)
		{
			i++;
		}

		RenderTexture.active = null;
	}
}
