#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// Declare structure to be used to pass data from C++ to Mono.
struct Circle
{
	Circle(int x, int y, int radius) : X(x), Y(y), Radius(radius) {}
	int X, Y, Radius;
};

CascadeClassifier _faceCascade;
String _windowName = "Unity OpenCV Interop Sample";
VideoCapture _capture;
int _scale = 1;

extern "C" int __declspec(dllexport) __stdcall  Init(int& outCameraWidth, int& outCameraHeight);

extern "C" void __declspec(dllexport) __stdcall  Close();

extern "C" void __declspec(dllexport) __stdcall SetScale(int scale);

extern "C" void __declspec(dllexport) __stdcall Detect(Circle * outFaces, int maxOutFacesCount, int& outDetectedFacesCount);