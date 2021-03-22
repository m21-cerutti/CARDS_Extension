
using System;
using System.IO;

using Plugin;

using UnityEngine;

/// <summary>
/// Video file frame provider. See FrameProvider.
/// Use video from opencv to get frames.
/// </summary>
public class VideoFileTexture : FrameProvider
{
	private IntPtr _video = IntPtr.Zero;

	public override void Init(VideoParameters parameters)
	{
		if(File.Exists(parameters.file_path_video))
		{
			_video = CARDSVideoPlugin.CreateVideoContextWrapped(parameters.file_path_video, parameters.camera_width, parameters.camera_height);
			Debug.Log("Open video " + parameters.file_path_video);
		}
		else
		{
			throw new Exception("File not exists " + Path.GetFullPath(parameters.file_path_video));
		}

		base.Init(parameters);
	}

	public override void Close()
	{
		if(_video != IntPtr.Zero)
		{
			Debug.Log("Close video.");
			CARDSVideoPlugin.FreeVideoContextWrapped(_video);
			_video = IntPtr.Zero;
		}
		base.Close();
	}

	public override bool GetFrame(out Frame fr)
	{
		bool valid = false;
		if(_video != IntPtr.Zero)
		{
			valid = CARDSVideoPlugin.GetFrameWrapped(_video, ref frame);
		}
		fr = frame;
		return valid;
	}
}
