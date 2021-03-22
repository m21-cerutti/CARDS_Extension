
using System.Runtime.InteropServices;

using Plugin;

using UnityEngine;

/// <summary>
/// Virtual camera frame provider. See FrameProvider.
/// </summary>
public class VirtualCameraTexture : FrameProvider
{
	private Camera _cam = null;
	private RenderTexture _render_texture = null;

	public override void Init(VideoParameters parameters)
	{
		//Need special objects, watchout for rename.
		_cam = GameObject.Find("CamTable").GetComponent<Camera>();
		_render_texture = Resources.Load("CameraTarget", typeof(RenderTexture)) as RenderTexture;

		Debug.Assert(_render_texture != null);
		Debug.Assert(_cam != null);

		base.Init(parameters);
		_cam.targetTexture = _render_texture;
	}

	public override void Close()
	{
		base.Close();
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
		frame.raw_data = pixelHandle.AddrOfPinnedObject();

		RenderTexture.active = null;
		fr = frame;
		return true;
	}

	public override Frame GetCopyFrame()
    {
		frameBackground = frame;
		return frameBackground;
	}
}
