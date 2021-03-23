
using Plugin;

using UnityEngine;

/// <summary>
/// Video parameters. Use for defining special media parameters. Can change during runtime.
/// </summary>
[CreateAssetMenu(fileName = "Param", menuName = "VideoParameters/Parameters", order = 1)]
public class VideoParameters : ScriptableObject
{
	public int camera_width = 256;
	public int camera_height = 256;


	public int requested_camera_fps = 60;

	public int starting_frame = 10;

	/// <summary>
	/// Use it for having OpenCV windows.
	/// </summary>
	public bool debug_cv = true;

	/// <summary>
	/// Use it for having datas in log folder. See more information on LogsDatas script and log folder in Unity documentation.
	/// </summary>
	public bool log_datas = false;

	/// <summary>
	/// Device choice, -1 for virtual, -2 for video, >=0 index for computer hardware if multiple
	/// </summary>
	public int device_index = -1;
	public bool UseWebcam
	{
		get
		{
			return device_index >= 0;
		}
	}

	public bool use_file_calibration = true;

	[ShowWhen("use_file_calibration", true)]
	public string path_subfolder_calibration;

	[ShowWhen("use_file_calibration", false)]
	public PoseParameters calibration;

	public FlipMode flip_mode = 0;

	/// <summary>
	/// The frequency of checktrack dll function.
	/// </summary>
	public int checktrack_frequency = 20;

	public bool use_detection = false;
	[ShowWhen("use_detection", true)]
	public Rect rect_detection = new Rect(0, 0, 1, 1);

	/// <summary>
	/// Used only in video media.
	/// </summary>
	[ShowWhen("device_index", "==-2")]
	public string file_path_video;
}
