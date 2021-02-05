#include "console.h"

using namespace cv;

int main(int argc, char** argv)
{
	TestDLL();

	VideoCapture cap;
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.
	if (!cap.open(0))
		return 0;
	for (;;)
	{
		Mat frame;
		cap >> frame;
		if (frame.empty()) break; // end of video stream
		imshow("this is you, smile! :)", frame);
		if (waitKey(10) == 27) break; // stop capturing by pressing ESC
	}
	// the camera will be closed automatically upon exit
	// cap.close();

	return 0;
}

void TestDLL()
{
	//Test DLL
	SetScale(2);
	cout << "Ok DLL." << endl;
}
