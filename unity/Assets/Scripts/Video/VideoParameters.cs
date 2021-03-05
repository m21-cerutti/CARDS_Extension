using System.Collections;
using System.Collections.Generic;

using UnityEngine;

[CreateAssetMenu(fileName = "Param", menuName = "VideoParameters/Parameters", order = 1)]
public class VideoParameters : ScriptableObject
{
	public int camera_width = 256;
	public int camera_height = 256;
	public int requested_camera_fps = 60;
	public int starting_frame = 10;
}
