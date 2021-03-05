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

	virtual public void Init(int width, int height, int fps)
	{
		frame.width = width;
		frame.height = height;
		frame.rawData = new IntPtr();
	}

	abstract public Frame GetFrame();

}
