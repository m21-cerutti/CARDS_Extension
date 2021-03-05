//
// SART Unity class implementation.
// Author : Joan Odicio
// Date : 02 February 2021
// Email: joan.odicio-vilchez@inria.fr
//
// Property of the INRIA Team POTIOC, there is no license applied to this project.
//

using Plugin;

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;

using UnityEngine;

public class ThreadedBehaviourInvokeSARPlugin : ThreadedBehaviourBase
{
	//[SerializeField]
	//private WebcamTexture cameraTextureWrapper;

	protected override void InternalStart()
	{
		//cameraTextureWrapper.Init(60);
	}

	/// <summary>
	/// Prepare an array which contains the raw data of the camera texture.
	/// </summary>
	protected override void InternalUpdate()
	{
		//Debug.Assert(cameraTextureWrapper.CamTexture != null, "camTexture must be instantiated first!");

		//cameraTextureWrapper.Update();
	}

	/// <summary>
	/// InternalChildLoop: Call the SARPlugin methods.
	/// Use this method only inside ChildThreadLoop only.
	/// </summary>
	protected override void InternalChildLoop()
	{
		/*
		if(cameraTextureWrapper.CamTexture != null && cameraTextureWrapper.TextureByteArray != null)
		{
			GCHandle arrayHandle = GCHandle.Alloc(cameraTextureWrapper.TextureByteArray, GCHandleType.Pinned);

			IntPtr arrayHandlePtr = arrayHandle.AddrOfPinnedObject();

			// Send the pointer of the camera texture (arrayHandlePtr) to the plugin when is needed

			// For this example, it won't be used:
			//Book[] bookList = GetExportedBooks(); // Code commented to not spam the console

			// Free the handle
			arrayHandle.Free();

			// unreference textureByteArray
			cameraTextureWrapper.UnreferenceTextureByteArray();
		}
		*/
	}

	public void StopCamera()
	{
		//cameraTextureWrapper.StopCamera();
	}

	/// <summary>
	/// Example of Marshaling for a struct list which contains a nested struct
	/// 
	/// <returns></returns>
	/*
	protected Book[] GetExportedBooks()
	{
		int arraySize = 0;

		IntPtr dataPtr = SARPlugin.GetPageExportListWrapped(ref arraySize);

		ExportedBook[] pointArr = new ExportedBook[arraySize];
		Book[] bookList = new Book[arraySize];

		int offset = 0;
		int vectorSize = Marshal.SizeOf(typeof(ExportedBook));
		long dataPtrSize = (IntPtr.Size == 4) ? dataPtr.ToInt32() : dataPtr.ToInt64();

		for(int i = 0; i < arraySize; i++)
		{
			pointArr[i] = (ExportedBook)Marshal.PtrToStructure(new IntPtr(dataPtrSize + offset), typeof(ExportedBook));

			bookList[i].Id = pointArr[i].Id;
			bookList[i].Mat = pointArr[i].Mat;

			int rectSize = pointArr[i].rectsSize;

			bookList[i].Rects = new Plugin.Rect[rectSize];

			for(int j = 0; j < rectSize; j++)
			{
				bookList[i].Rects[j] = pointArr[i].Rects[j];

				Debug.Log("Rect: x:" + pointArr[i].Rects[j].id + ": " + pointArr[i].Rects[j].x +
							" , y: " + pointArr[i].Rects[j].y +
							" , width: " + pointArr[i].Rects[j].width +
							" , height: " + pointArr[i].Rects[j].height);
			}

			offset += vectorSize;
		}

		Marshal.FreeCoTaskMem(dataPtr);

		return bookList;
	}
	*/
}