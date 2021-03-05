//
// SART Unity class implementation.
// Author : Joan Odicio
// Date : 02 February 2021
// Email: joan.odicio-vilchez@inria.fr
//
// Property of the INRIA Team POTIOC, there is no license applied to this project.
//

using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using Plugin;

using UnityEngine;
using UnityEngine.UI;

public class WebcamTexture : VideoProvider
{
	private WebCamTexture _cam_texture;

	override public void Init(int width, int height, int fps)
	{
		_cam_texture = new WebCamTexture();
		_cam_texture.requestedFPS = fps;
		_cam_texture.requestedWidth = width;
		_cam_texture.requestedHeight = height;

		WebCamDevice[] devices = WebCamTexture.devices;
		if(devices.Length > 0)
		{
			// If you have more than 2 cameras, ex: a webcam conected in a laptop or a virtual camera like OBS Camera
			// You should search in advance what is the index of the wanted camera, in the devices array.
			// Or you can use directly the name of the device once you had found its name.
			int deviceIndex = 0;
			_cam_texture.deviceName = devices[deviceIndex].name;
			_cam_texture.Play();

			base.Init(_cam_texture.width, _cam_texture.height, fps);
			frame.is_flipped = true;
		}
	}

	public override bool GetFrame(out Frame fr)
	{
		if(_cam_texture.isPlaying)
		{
			pixels = _cam_texture.GetPixels32();

			//Texture memory
			GCHandle pixelHandle = GCHandle.Alloc(pixels, GCHandleType.Pinned);
			frame.rawData = pixelHandle.AddrOfPinnedObject();

			fr = frame;
			return true;
		}

		Debug.LogError("No webcam");
		fr = new Frame();
		return false;
	}

	public void StopCamera()
	{
		if(_cam_texture.isPlaying)
		{
			_cam_texture.Stop();
		}
	}
}
