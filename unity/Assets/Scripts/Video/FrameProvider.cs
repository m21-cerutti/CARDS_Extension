using System;

using Plugin;

using UnityEngine;

/// <summary>
/// Parent class that give the frame for a particular media.
/// </summary>
public abstract class FrameProvider
{
	protected Frame frame;

	//Buffer
	protected Color32[] pixels;

	/// <summary>
	/// Init the context and intern data.
	/// </summary>
	/// <param name="parameters">Video parameters unique to a media.</param>
	public virtual void Init(VideoParameters parameters)
	{
		frame.width = parameters.camera_width;
		frame.height = parameters.camera_height;
		frame.raw_data = new IntPtr();
		frame.flip_mode = parameters.flip_mode;

		pixels = new Color32[parameters.camera_width * parameters.camera_height];
	}

	/// <summary>
	/// Close the context and intern data.
	/// </summary>
	public virtual void Close()
	{
	}

	/// <summary>
	/// Give the frame ready to be sent to OpenCV DLL
	/// </summary>
	/// <param name="fr">The frame structure filled.</param>
	/// <returns>True if suceed, false if fail.</returns>
	public abstract bool GetFrame(out Frame fr);

}
