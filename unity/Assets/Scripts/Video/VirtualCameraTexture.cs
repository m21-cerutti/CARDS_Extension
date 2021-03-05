using System.Collections;
using System.Collections.Generic;

using UnityEngine;
using System.Runtime.InteropServices;
using System;
using Plugin;

public class VirtualCameraTexture : VideoProvider
{
	private Camera _cam = null;
	private RenderTexture _render_texture = null;

	override public void Init(VideoParameters parameters)
	{
		_cam = GameObject.Find("CamTable").GetComponent<Camera>();
		_render_texture = Resources.Load("CameraTarget", typeof(RenderTexture)) as RenderTexture;
		Debug.Assert(_render_texture != null);
		Debug.Assert(_cam != null);

		base.Init(parameters);
		_cam.targetTexture = _render_texture;
	}

	public override bool GetFrame(out Frame fr)
	{
		Texture2D tex = new Texture2D(_render_texture.width, _render_texture.height, TextureFormat.RGB24, false);
		RenderTexture.active = _render_texture;
		tex.ReadPixels(new Rect(0, 0, _render_texture.width, _render_texture.height), 0, 0);
		tex.Apply();
		pixels = tex.GetPixels32();

		//Texture memory
		GCHandle pixelHandle = GCHandle.Alloc(pixels, GCHandleType.Pinned);
		frame.rawData = pixelHandle.AddrOfPinnedObject();

		RenderTexture.active = null;
		fr = frame;

		return true;
	}
}
