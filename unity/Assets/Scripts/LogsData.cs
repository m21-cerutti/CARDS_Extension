using System;
using System.Collections;
using System.IO;
using System.Text;

using UnityEngine;

using Utility.Singleton;

/// <summary>
/// Singleton class that permit to log personalized data like erors and performances.
/// Folder of destination depend on build, see Unity log documentation (https://docs.unity3d.com/Manual/LogFiles.html)
/// </summary>
public class LogsData : SingletonBehaviour<LogsData>
{
	/// <summary>
	/// Folder in Logs that contained datas.
	/// </summary>
	[SerializeField]
	private string _log_directory = "LogsData";

	private StreamWriter _log_errors;
	private StreamWriter _log_perfs;
	private Coroutine _corout_endframe;
	private StringBuilder _buffer_errors = new StringBuilder();
	private StringBuilder _buffer_perfs = new StringBuilder();


	private void Start()
	{
		System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");
	}

	public void OnEnable()
	{
		CreateStreamLog();
		if(_corout_endframe == null)
		{
			_corout_endframe = StartCoroutine(DebugFrameCoroutine());
		}
	}

	public void OnDisable()
	{
		if(_corout_endframe != null)
		{
			StopCoroutine(_corout_endframe);
		}

		CloseStreamLog();
	}

	/// <summary>
	/// Init errors file with header.
	/// </summary>
	/// <param name="maxTargets"> Number of columns for header.</param>
	public void InitTargetErrorHeader(int maxTargets)
	{
		for(int i = 0; i < maxTargets; i++)
		{
			_buffer_errors.Append(i).Append(";");
		}

		_buffer_errors.Append("\n");
	}

	/// <summary>
	/// Append error message to the actual frame.
	/// </summary>
	/// <param name="message">The message to append.</param>
	public void DebugTargetsError(string message)
	{
		_buffer_errors.Append(message + ";\t");
	}

	/// <summary>
	/// Append performance message to the actual frame.
	/// </summary>
	/// <param name="message">The message to append.</param>
	public void DebugFramePerf(string message)
	{
		_buffer_perfs.Append(message);
	}


	/// <summary>
	/// Run action and append time performance to the actual frame.
	/// </summary>
	/// <param name="action">The action to run.</param>
	public void DebugFramePerf(Action action)
	{
		System.Diagnostics.Stopwatch stopwatch = System.Diagnostics.Stopwatch.StartNew();
		action();
		stopwatch.Stop();
		DebugFramePerf(stopwatch.Elapsed.TotalMilliseconds.ToString());
	}

	/// <summary>
	/// Write to file's logs at end of frame.
	/// </summary>
	/// <returns>IEnumerator</returns>
	private IEnumerator DebugFrameCoroutine()
	{
		while(isActiveAndEnabled)
		{
			yield return new WaitForEndOfFrame();
			_log_errors.WriteLine(_buffer_errors.ToString());
			_log_perfs.WriteLine(_buffer_perfs.ToString());
			_buffer_perfs.Clear();
			_buffer_errors.Clear();
		}
	}

	/// <summary>
	/// Create the log's streams.
	/// </summary>
	private void CreateStreamLog()
	{
		_log_directory = Path.Combine(Path.GetDirectoryName(Application.consoleLogPath), _log_directory);
		Directory.CreateDirectory(_log_directory);
		Debug.Log("Directory logs:" + _log_directory);

		string date = DateTime.Now.ToString("yyyy-MM-dd_HH-mm-ss");

		string log_errors_path = Path.Combine(_log_directory, date + "_errors.csv");
		if(!File.Exists(log_errors_path))
		{
			_log_errors = new StreamWriter(log_errors_path, false, Encoding.UTF8);
		}
		else
		{
			Debug.LogError("File already exist : " + log_errors_path);
		}

		string log_perfs_path = Path.Combine(_log_directory, date + "_perfs.csv");
		if(!File.Exists(log_perfs_path))
		{
			_log_perfs = new StreamWriter(log_perfs_path, false, Encoding.UTF8);
		}
		else
		{
			Debug.LogError("File already exist : " + log_perfs_path);
		}
	}

	/// <summary>
	/// Close the log's streams.
	/// </summary>
	private void CloseStreamLog()
	{
		_log_perfs.Close();
		_log_errors.Close();
	}
}
