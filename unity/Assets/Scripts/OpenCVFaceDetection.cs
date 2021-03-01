using System.Collections.Generic;
using System.Runtime.InteropServices;

using UnityEngine;

public class OpenCVFaceDetection : MonoBehaviour
{
	public static List<Vector2> NormalizedFacePositions { get; private set; }
	public static Vector2 camera_resolution;

	/// <summary>
	/// Downscale factor to speed up detection.
	/// </summary>
	private const int _detection_down_scale = 1;

	private bool _ready;
	private int _max_face_detect_count = 5;
	private CvCircle[] _faces;

	void Start()
	{
		int camWidth = 0, camHeight = 0;
		int result = OpenCVWrapper.Init(ref camWidth, ref camHeight);
		if(result < 0)
		{
			if(result == -1)
			{
				Debug.LogWarningFormat("[{0}] Failed to find cascades definition.", GetType());
			}
			else if(result == -2)
			{
				Debug.LogWarningFormat("[{0}] Failed to open camera stream.", GetType());
			}

			return;
		}

		camera_resolution = new Vector2(camWidth, camHeight);
		_faces = new CvCircle[_max_face_detect_count];
		NormalizedFacePositions = new List<Vector2>();
		OpenCVWrapper.SetScale(_detection_down_scale);
		_ready = true;
	}

	void OnApplicationQuit()
	{
		if(_ready)
		{
			OpenCVWrapper.Close();
		}
	}

	void Update()
	{
		if(!_ready)
			return;

		int detectedFaceCount = 0;
		unsafe
		{
			fixed(CvCircle* outFaces = _faces)
			{
				OpenCVWrapper.Detect(outFaces, _max_face_detect_count, ref detectedFaceCount);
			}
		}

		NormalizedFacePositions.Clear();
		for(int i = 0; i < detectedFaceCount; i++)
		{
			NormalizedFacePositions.Add(new Vector2((_faces[i].x * _detection_down_scale) / camera_resolution.x, 1f - ((_faces[i].y * _detection_down_scale) / camera_resolution.y)));
		}
	}
}

// Define the functions which can be called from the .dll.
internal static class OpenCVWrapper
{
	[DllImport("cards_rgbtrack")]
	internal static extern int Init(ref int outCameraWidth, ref int outCameraHeight);

	[DllImport("cards_rgbtrack")]
	internal static extern int Close();

	[DllImport("cards_rgbtrack")]
	internal static extern int SetScale(int downscale);

	[DllImport("cards_rgbtrack")]
	internal unsafe static extern void Detect(CvCircle* outFaces, int maxOutFacesCount, ref int outDetectedFacesCount);
}

// Define the structure to be sequential and with the correct byte size (3 ints = 4 bytes * 3 = 12 bytes)
[StructLayout(LayoutKind.Sequential, Size = 12)]
public struct CvCircle
{
	public int x, y, radius;
}
