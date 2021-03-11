using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.IO;
using Utility.Singleton;
using System.Text;

public class LogsData : SingletonBehaviour<LogsData>
{
	static public string prefix_log_performance = "Log_Perf : ";
	static public string prefix_log_errors = "Log_Error : ";

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

	void OnEnable()
	{
		CreateStreamLog();
		Application.logMessageReceived += HandleLog;
		if(_corout_endframe == null)
			_corout_endframe = StartCoroutine(DebugFrameCoroutine());
	}

	void OnDisable()
	{
		if(_corout_endframe != null)
			StopCoroutine(_corout_endframe);
		Application.logMessageReceived -= HandleLog;
		CloseStreamLog();
	}

	public void InitTargetErrorHeader(int maxTargets)
	{
		for(int i = 0; i < maxTargets; i++)
			_buffer_errors.Append(i).Append(";");
		_buffer_errors.Append("\n");
	}

	public void DebugTargetsError(string message)
	{
		_buffer_errors.Append(message + ";\t");
	}

	public void DebugFramePerf(string message)
	{
		_buffer_perfs.Append(message);
	}

	public void DebugFramePerf(Action action)
	{
		System.Diagnostics.Stopwatch stopwatch = System.Diagnostics.Stopwatch.StartNew();
		action();
		stopwatch.Stop();
		DebugFramePerf(stopwatch.Elapsed.TotalMilliseconds.ToString());
	}

	private IEnumerator DebugFrameCoroutine()
	{
		while(isActiveAndEnabled)
		{
			yield return new WaitForEndOfFrame();
			Debug.Log(prefix_log_errors + _buffer_errors.ToString());
			Debug.Log(prefix_log_performance + _buffer_perfs.ToString());
			_buffer_perfs.Clear();
			_buffer_errors.Clear();
		}
	}

	private void HandleLog(string logString, string stackTrace, LogType type)
	{
		logString = logString.Remove(logString.Length - "\r\n".Length, "\r\n".Length);
		if(logString.StartsWith(prefix_log_performance))
		{
			_log_perfs.WriteLine(logString.Remove(0, prefix_log_performance.Length));
		}

		if(logString.StartsWith(prefix_log_errors))
		{
			_log_errors.WriteLine(logString.Remove(0, prefix_log_errors.Length));
		}
	}

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

	private void CloseStreamLog()
	{
		_log_perfs.Close();
		_log_errors.Close();
	}
}
