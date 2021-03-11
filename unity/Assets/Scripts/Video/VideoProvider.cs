using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Plugin;

public abstract class VideoProvider
{
	protected Frame frame;

	//Buffer
	protected Color32[] pixels;

	virtual public void Init(VideoParameters parameters)
	{
		frame.width = parameters.camera_width;
		frame.height = parameters.camera_height;
		frame.raw_data = new IntPtr();
		pixels = new Color32[parameters.camera_width * parameters.camera_height];
		frame.is_flipped_x = parameters.is_flipped_x;
		frame.is_flipped_y = parameters.is_flipped_y;
	}

	abstract public bool GetFrame(out Frame fr);
}
