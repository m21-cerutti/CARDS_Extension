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

	override public void Init(VideoParameters parameters)
	{
		_cam_texture = new WebCamTexture();
		_cam_texture.requestedFPS = parameters.requested_camera_fps;
		_cam_texture.requestedWidth = parameters.camera_width;
		_cam_texture.requestedHeight = parameters.camera_height;

		WebCamDevice[] devices = WebCamTexture.devices;
		if(devices.Length > 0)
		{
			_cam_texture.deviceName = devices[parameters.device_index].name;
			_cam_texture.Play();

			parameters.requested_camera_fps = (int)_cam_texture.requestedFPS;
			parameters.camera_width = _cam_texture.width;
			parameters.camera_height = _cam_texture.height;

			base.Init(parameters);
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
			frame.raw_data = pixelHandle.AddrOfPinnedObject();

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
