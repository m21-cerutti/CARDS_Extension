using System;
using System.IO;

using Plugin;

using UnityEngine;

using Xml2CSharp;

/// <summary>
/// See DebugTargets.
/// Use a XML File associated with the video for debugging real positions.
/// </summary>
public class DebugTargetsVideo : DebugTargets
{
	private OpencvStorage _xmlroot = null;

	protected override bool GetRealPositionTarget(int id, out Vector3 pos, out StateTracker state)
	{
		if(_xmlroot == null)
		{
			pos = Vector3.zero;
			state = StateTracker.Live;
			return true;
		}

		pos = Vector3.zero;
		state = StateTracker.Undefined;
		XMLFrame frame = _xmlroot.ListFrames.Frames.Find(delegate (XMLFrame f)
	   {
		   return f.Number == tracking.NbFrame.ToString();
	   });

		if(frame != null)
		{
			XMLTarget target = frame.ListTargets.Targets.Find(delegate (XMLTarget t)
			{
				return t.Id == id.ToString();
			});

			if(target == null)
			{
				return false;
			}
			if(target.State == "Lost")
			{
				state = StateTracker.Lost;
				return true;
			}

			state = StateTracker.Live;
			Rect rect = new Rect
			{
				x = float.Parse(target.X),
				y = float.Parse(target.Y),
				width = float.Parse(target.Width),
				height = float.Parse(target.Height)
			};
			Vector2 screenpos = GetCenterScreenTarget(rect);
			Vector3 prevision = GetScreenToWorldSpace(screenpos);
			pos = prevision;
			return true;
		}
		return false;
	}

	protected override void Start()
	{
		base.Start();
		if(parameters.device_index != -2)
		{
			throw new System.Exception("Use a video with this script !");
		}

		if(parameters.log_datas)
		{
			ParseXmlFile();
		}
	}

	private void ParseXmlFile()
	{
		string path = Path.ChangeExtension(parameters.file_path_video, ".xml");
		if(File.Exists(parameters.file_path_video))
		{
			_xmlroot = OpencvStorage.Load(path);
		}
		else
		{
			throw new Exception("File not exists " + Path.GetFullPath(path));
		}
	}
}
