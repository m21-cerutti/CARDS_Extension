using System.Threading;

using UnityEngine;
using UnityEngine.UI;


/// <summary>
/// Based on: https://80.lv/articles/simple-multithreading-for-unity/
/// </summary>
public class ThreadedBehaviourBase : MonoBehaviour
{
	#region THREAD_RELATED_ATTRIBUTES
	private Thread childThread = null;
	private EventWaitHandle childThreadWait = new EventWaitHandle(true, EventResetMode.ManualReset);
	private EventWaitHandle mainThreadWait = new EventWaitHandle(true, EventResetMode.ManualReset);
	#endregion

	private void Awake()
	{
		childThread = new Thread(ChildThreadLoop);
		childThread.Start();
	}

	virtual protected void InternalStart()
	{

	}

	private void Start()
	{
		InternalStart();
	}

	virtual protected void InternalUpdate()
	{

	}

	private void Update()
	{
		// Execute this line of code first
		// It send a signal to indicate that the ChildThread can continue
		// Attention to not write shared data in both threads, this code is not thread safe
		childThreadWait.Set();

		InternalUpdate();
	}

	private void OnDestroy()
	{
		childThread.Abort();
	}

	/// <summary>
	/// Use this method only inside ChildThreadLoop, if not it will trigger an assert during runtime
	/// </summary>
	virtual protected void InternalChildLoop()
	{

	}

	/// <summary>
	/// This method is intented to be used as a service only, which always provide data ready for the mainthread
	/// Normally the go and back from the plugin in Open CV is less han 16 ms, if you don't use a heavy algorithm
	/// In case InternalChildLoop takes more than 16 ms, reduce the framerate in Application.targetFrameRate 
	/// </summary>
	private void ChildThreadLoop()
	{
		childThreadWait.Reset();
		childThreadWait.WaitOne();

		while(true)
		{
			childThreadWait.Reset();

			InternalChildLoop();

			WaitHandle.SignalAndWait(mainThreadWait, childThreadWait);
		}
	}
}
