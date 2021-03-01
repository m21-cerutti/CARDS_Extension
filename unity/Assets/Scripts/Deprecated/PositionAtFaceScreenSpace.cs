using UnityEngine;

public class PositionAtFaceScreenSpace : MonoBehaviour
{
	private float _cam_distance;

	void Start()
	{
		_cam_distance = Vector3.Distance(Camera.main.transform.position, transform.position);
	}

	void Update()
	{

		if(OpenCVFaceDetection.NormalizedFacePositions.Count == 0)
			return;

		transform.position = Camera.main.ViewportToWorldPoint(new Vector3(OpenCVFaceDetection.NormalizedFacePositions[0].x, OpenCVFaceDetection.NormalizedFacePositions[0].y, _cam_distance));

	}
}