
using Plugin;

using UnityEngine;

/// <summary>
/// See DebugTargets.
/// Use GameObjects to compare. 
/// Watchout in the first frame for registering GameObjects, the center of Targets rect must be on the GameObjects precisely.
/// Otherwise error in registering will occure.
/// </summary>
public class DebugTargetsVirtual : DebugTargets
{
	private int _previous_nb_target = 0;
	private GameObject[] _game_targets;

	protected override bool GetRealPositionTarget(int id, out Vector3 pos, out StateTracker state)
	{
		pos = _game_targets[id].transform.position;
		//TODO Pos limits by camera
		state = StateTracker.Live;
		return true;
	}

	protected override void Start()
	{
		base.Start();
		if(parameters.UseWebcam)
		{
			throw new System.Exception("Never use webcam with this script !");
		}
		_previous_nb_target = 0;
		_game_targets = new GameObject[tracking.MaxTargets];
	}

	protected override void Update()
	{
		if(tracking.NbTargets != _previous_nb_target)
		{
			RegisterGameObjectVirtual();
		}

		base.Update();
	}

	private void RegisterGameObjectVirtual()
	{
		foreach(Target t in tracking.GetTargets())
		{
			if(t.state != StateTracker.Undefined)
			{
				Vector2 point = GetCenterScreenTarget(t);
				Ray ray = Camera.main.ScreenPointToRay(point);
				if(Physics.Raycast(ray, out RaycastHit hit, 5.0f))
				{
					_game_targets[t.id] = hit.collider.gameObject;
				}
			}
		}
		_previous_nb_target = tracking.NbTargets;
	}
}
