#include "console.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	TestDLL();
	TestWebcam();
	TestWorkflowWebcam();
}

void TestWebcam()
{

	std::cerr << "Opening camera...\r";

	VideoCapture cap;
	// open the default camera, use something different from 0 otherwise;
	// Check VideoCapture documentation.
	if (!cap.open(0, cv::CAP_ANY))
	{
		std::cerr << "Test camera failed ! Can't open." << std::endl;
		return;
	}

	for (int i = 0; i < 200; i++)
	{
		Mat frame;
		cap >> frame;
		if (frame.empty())
		{
			std::cerr << "Warning! Empty frame." << std::endl;
			break;
		}
		//imshow("Test webcam", frame);
		//if (waitKey(10) == 27) break; // stop capturing by pressing ESC
	}
	destroyAllWindows();
	cap.release();
	cout << "End test Webcam." << endl;
}

void TestDLL()
{
	//Test DLL
	Init();
	Close();
	cout << "End test DLL." << endl;
}

void TestWorkflowWebcam()
{
	cout << "End test WorkflowWebcam." << endl;
}
