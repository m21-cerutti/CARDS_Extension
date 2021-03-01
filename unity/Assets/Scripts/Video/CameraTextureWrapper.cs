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

using UnityEngine;
using UnityEngine.UI;

[Serializable]
public class CameraTextureWrapper
{
	[SerializeField]
	private RawImage _input_image = default;

	[SerializeField]
	private RawImage _output_image = default;
	public RawImage OutputImage => _output_image;

	private WebCamTexture _cam_texture;
	public WebCamTexture CamTexture => _cam_texture;

	private byte[] _texture_byte_array = default;
	public byte[] TextureByteArray => _texture_byte_array;

	private int _camera_width = 1920;
	private int _camera_height = 1080;
	private int _requested_camera_fps = 60;

	public void Init(int framerate)
	{
		Application.targetFrameRate = framerate;

		_cam_texture = new WebCamTexture();

		_cam_texture.requestedFPS = _requested_camera_fps;
		_cam_texture.requestedWidth = _camera_width;
		_cam_texture.requestedHeight = _camera_height;

		WebCamDevice[] devices = WebCamTexture.devices;

		if(devices.Length > 0)
		{
			// If you have more than 2 cameras, ex: a webcam conected in a laptop or a virtual camera like OBS Camera
			// You should search in advance what is the index of the wanted camera, in the devices array.
			// Or you can use directly the name of the device once you had found its name.
			int deviceIndex = 0;
			_cam_texture.deviceName = devices[deviceIndex].name;

			_cam_texture.Play();
		}
	}

	public void Update()
	{
		if(_cam_texture.isPlaying && _texture_byte_array == null)
		{
			Texture2D bufferTexture = new Texture2D(_camera_width, _camera_height, TextureFormat.RGBA32, false);

			// Set the new Texture2D with the camera texture
			Color32[] pixels32 = _cam_texture.GetPixels32();
			bufferTexture.SetPixels32(pixels32);
			bufferTexture.Apply(true, false);

			// Get the RawTextureData of the bufferTexture
			_texture_byte_array = bufferTexture.GetRawTextureData();

			//  Clean the memory
			pixels32 = null;

			UnityEngine.Object.Destroy(bufferTexture);
		}
	}

	public void StopCamera()
	{
		if(CamTexture.isPlaying)
		{
			CamTexture.Stop();
		}
	}

	public void UnreferenceTextureByteArray()
	{
		_texture_byte_array = null;
	}

	public void SetOutputTexture(Texture2D texture)
	{
		if(_output_image != null)
		{
			_output_image.texture = texture;
		}
	}

}
