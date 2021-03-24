
using System.Collections.Generic;

using Plugin;

using UnityEngine;

/// <summary>
/// Handle debugging targets position from tracking and comparaison to real data.
/// Specific to media.
/// </summary>
[RequireComponent(typeof(Tracking))]
public abstract class DebugTargets : MonoBehaviour
{
	protected VideoParameters parameters;
	protected Tracking tracking;
	protected Camera camera_table;

	protected Dictionary<int, Vector3> estimate_pos = new Dictionary<int, Vector3>();

	/// <summary>
	/// Permit to get real data, specific to implementation.
	/// </summary>
	/// <param name="id">The target id.</param>
	/// <param name="pos">Output of real data position.</param>
	/// <param name="state">Output of real tracker state.</param>
	/// <returns>True if there is Data available, false otherwise.</returns>
	protected abstract bool GetRealPositionTarget(int id, out Vector3 pos, out StateTracker state);

	protected virtual void Start()
	{
		tracking = GetComponent<SingleThread>();
		parameters = tracking.Parameters;
		if(parameters.log_datas)
		{
			LogsData.Instance.InitTargetErrorHeader(tracking.MaxTargets);
		}

		camera_table = Camera.main;
	}

	protected virtual void Update()
	{
		foreach(Target t in tracking.GetTargets())
		{
			SetEstimateWorldPosition(t);
		}
		if(tracking.NbTargets > 0 && !parameters.UseWebcam)
		{
			CompareEstimationtoReal();
		}
	}

	protected void CompareEstimationtoReal()
	{
		foreach(Target t in tracking.GetTargets())
		{
			Vector3 real_position;
			StateTracker state;
			if(GetRealPositionTarget(t.id, out real_position, out state))
			{
				if(t.state == state)
				{
					switch(t.state)
					{
						case StateTracker.Undefined:
							if(parameters.log_datas)
							{
								LogsData.Instance.DebugTargetsError("nan");
							}

							break;
						case StateTracker.Live:
							{
								Vector3 prevision;
								estimate_pos.TryGetValue(t.id, out prevision);
								if(prevision != null)
								{
									float error = Vector3.Distance(real_position, prevision);
									if(parameters.log_datas)
									{
										LogsData.Instance.DebugTargetsError(error.ToString());
									}
								}
							}
							break;
						default:
							if(parameters.log_datas)
							{
								LogsData.Instance.DebugTargetsError("0");
							}
							break;
					}

				}
				else
				{
					if(parameters.log_datas)
					{
						LogsData.Instance.DebugTargetsError(Mathf.Infinity.ToString());
					}
				}
			}


		}
	}

	private void SetEstimateWorldPosition(Target t)
	{
		if(t.state != StateTracker.Undefined)
		{
			Matrix4x4f mat = CARDSTrackingPlugin.EstimatePoseWrapped(ref t, ref parameters.calibration);
			//float depth = mat.c_23 * parameters.calibration.dist_cam;

			//TODO Debug purpose
			float depth = parameters.calibration.dist_cam;

			Vector2 screenpoint = GetCenterScreenTarget(t);
			Vector3 worldPoint = GetScreenToWorldSpace(screenpoint, depth);

			/* REAL POSE TODO
			Vector3 worldPoint = new Vector3(mat.c_03, mat.c_23, mat.c_13);
			*/
			//Debug.Log(worldPoint);
			if(!estimate_pos.ContainsKey(t.id))
			{
				estimate_pos[t.id] = Vector3.zero;
			}
			estimate_pos[t.id] = Vector3.Slerp(estimate_pos[t.id], worldPoint, 0.4f);
		}
	}

	#region Utilities methods

	/// <summary>
	/// Get from an Unityscreen point with depth the coresponding point in Unity World Space.
	/// </summary>
	/// <param name="point">The screen point.</param>
	/// <param name="depth">The depth set.</param>
	/// <returns></returns>
	virtual protected Vector3 GetScreenToWorldSpace(Vector2 point, float depth = -1)
	{
		if(depth < 0)
		{
			depth = parameters.calibration.dist_cam;
		}

		return camera_table.ScreenToWorldPoint(new Vector3(point.x, point.y, depth));
	}

	protected Vector2 GetCenterScreenTarget(Target t)
	{
		return GetCenterScreenTarget(t.rect);
	}

	protected Vector2 GetCenterScreenTarget(RectStruct rect)
	{
		return new Vector2(rect.x + (rect.width / 2.0f), (rect.y + (rect.height / 2.0f)));
	}

	protected Vector2 GetCenterScreenTarget(Rect rect)
	{
		return new Vector2(rect.x + ((float)(rect.width) / 2.0f), (rect.y + ((float)(rect.height) / 2.0f)));
	}

	#endregion

#if UNITY_EDITOR

	private void OnDrawGizmos()
	{
		//Landmark debug
		Gizmos.color = Color.white;
		Gizmos.DrawWireSphere(GetScreenToWorldSpace(new Vector2(0, 0)), 0.05f);
		Gizmos.color = Color.cyan;
		Gizmos.DrawWireSphere(GetScreenToWorldSpace(new Vector2(camera_table.pixelWidth, 0)), 0.05f);
		Gizmos.DrawWireSphere(GetScreenToWorldSpace(new Vector2(0, camera_table.pixelHeight)), 0.05f);
		Gizmos.DrawWireSphere(GetScreenToWorldSpace(new Vector2(camera_table.pixelWidth, camera_table.pixelHeight)), 0.05f);

		//Detection zone debug
		if(parameters.use_detection)
		{
			Vector3 pt1 = GetScreenToWorldSpace(parameters.rect_detection.min);
			Vector3 pt2 = GetScreenToWorldSpace(parameters.rect_detection.max);

			Gizmos.color = Color.black;
			Vector3 pt3 = pt1;
			pt3.x = pt2.x;
			Vector3 pt4 = pt1;
			pt4.z = pt2.z;
			Gizmos.DrawWireSphere(pt1, 0.05f);
			Gizmos.DrawWireSphere(pt2, 0.05f);
			Gizmos.DrawWireSphere(pt3, 0.05f);
			Gizmos.DrawWireSphere(pt4, 0.05f);
		}

		//Targets debug
		if(tracking != null)
		{
			foreach(Target t in tracking.GetTargets())
			{
				if(t.state != StateTracker.Undefined)
				{
					Color col;
					Vector3 real_position;
					StateTracker state;
					if(GetRealPositionTarget(t.id, out real_position, out state))
					{
						if(t.state == state && (state == StateTracker.Lost || state == StateTracker.Occluded || state == StateTracker.OutOfCamera))
						{
							col = Color.yellow;
						}
						else if(t.state == state)
						{
							col = Color.green;
						}
						else
						{
							col = Color.red;
							Debug.LogWarning("Not same state in target " + t.id);
							Debug.LogWarning("Real " + state);
							Debug.LogWarning("Tracker " + t.state);
						}
					}
					else
					{
						col = Color.gray;
					}
					Vector3 prevision;
					estimate_pos.TryGetValue(t.id, out prevision);
					if(prevision != null)
					{
						Gizmos.color = col;
						Gizmos.DrawWireSphere(prevision, 0.1f);
					}
				}

			}
		}
	}

#endif
}
