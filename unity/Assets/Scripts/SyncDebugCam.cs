using System.Collections;
using System.Collections.Generic;

using UnityEditor;

using UnityEngine;

public class SyncDebugCam : MonoBehaviour
{

	[SerializeField]
	private Camera cam_to_copy;

	// Start is called before the first frame update
	void Start()
	{
		SyncCamTable();

	}

	[ContextMenu("Sync cams")]
	private void SyncCamTable()
	{
		if(this.tag != "EditorOnly")
		{
			Debug.LogError("Use only on editor object");
			return;
		}
		Camera cam = GetComponent<Camera>();
		EditorUtility.CopySerialized(cam_to_copy, cam);
		EditorUtility.CopySerialized(cam_to_copy.transform, transform);
		cam.targetTexture = null;

		Screen.SetResolution(cam_to_copy.targetTexture.width, cam_to_copy.targetTexture.height, false);
	}
}
