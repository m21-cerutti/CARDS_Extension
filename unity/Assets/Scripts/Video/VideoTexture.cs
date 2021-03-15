using System.Collections;
using System.Collections.Generic;

using UnityEngine;
using System.Runtime.InteropServices;
using System.IO;
using Plugin;
using System;

public class VideoTexture : VideoProvider
{
	IntPtr _video = IntPtr.Zero;

	override public void Init(VideoParameters parameters)
	{
		if(File.Exists(parameters.file_path_video))
		{
			_video = CARDSVideoPlugin.CreateVideoContextWrapped(parameters.file_path_video, parameters.camera_width, parameters.camera_height);
			Debug.Log("Open video " + parameters.file_path_video);
		}
		else
			throw new Exception("File not exists " + Path.GetFullPath(parameters.file_path_video));
		base.Init(parameters);
	}

	override public void Close()
	{
		if(_video != IntPtr.Zero)
		{
			Debug.Log("Close video.");
			CARDSVideoPlugin.FreeVideoContextWrapped(_video);
			_video = IntPtr.Zero;
		}
		base.Close();
	}

	override public bool GetFrame(out Frame fr)
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
