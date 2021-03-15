using System.Collections;
using System.Collections.Generic;

using UnityEngine;

using Plugin;

[CreateAssetMenu(fileName = "Param", menuName = "VideoParameters/Parameters", order = 1)]
public class VideoParameters : ScriptableObject
{
	public int camera_width = 256;
	public int camera_height = 256;

	public int requested_camera_fps = 60;
	public int starting_frame = 10;

	//-1 for virtual, -2 for video, >=0 index computer if multiple
	public int device_index = -1;
	public bool UseWebcam { get => device_index >= 0; }

	public FlipMode flip_mode = 0;

	public int checktrack_frequency = 20;
	public bool use_detection = false;

	[ShowWhen("use_detection", true)]
	public Rect rect_detection = new Rect(0, 0, 1, 1);

	[ShowWhen("device_index", "==-2")]
	public string file_path_video;
}
