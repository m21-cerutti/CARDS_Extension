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

	override public void Init(int width, int height, int fps)
	{
		_cam = GameObject.Find("CamTable").GetComponent<Camera>();
		_render_texture = Resources.Load("CameraTarget", typeof(RenderTexture)) as RenderTexture;
		Debug.Assert(_render_texture != null);
		Debug.Assert(_cam != null);

		base.Init(_render_texture.width, _render_texture.height, fps);
		_cam.targetTexture = _render_texture;
	}

	public override Frame GetFrame()
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

		return frame;
	}
}
