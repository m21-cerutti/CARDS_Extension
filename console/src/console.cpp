#include "console.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	TestDLL();
	//TestWebcam();
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
	cv::destroyAllWindows();
	cap.release();
	std::cout << "End test Webcam." << endl;
}

void TestDLL()
{
	//Test DLL
	Init();
	Close();
	std::cout << "End test DLL." << endl;
}

void TestWorkflowWebcam()
{
	VideoCapture cap;
	if (!cap.open(0, cv::CAP_ANY))
	{
		std::cerr << "Test WorkflowWebcam failed ! Can't open." << std::endl;
		return;
	}

	Frame fr;
	int nbtargets = 0, maxTargets = 5;
	Target targets[5];

	for (int i = 0;; i++)
	{
		Mat frame;
		cap >> frame;
		if (frame.empty())
		{
			std::cerr << "Warning! Empty frame." << std::endl;
			break;
		}
		//frame.convertTo(frame, CV_8UC4);
		cvtColor(frame, frame, CV_BGR2RGBA);

		//imshow("Test", frame);
		//if (waitKey(50) == 27) break;

		if (i == 50)
		{
			Init();

			//Copy frame data to test struct pass
			//TODO displace to Init ?
			fr.height = frame.rows;
			fr.width = frame.cols;
			fr.rawData = new Color32[fr.height * fr.width];
			memcpy(fr.rawData, frame.data, 4 * frame.total());

			//Debug
			Mat texture(fr.height, fr.width, CV_8UC4, fr.rawData);
			cv::imshow("Init", texture);
		}
		else if (i == 100)
		{
			memcpy(fr.rawData, frame.data, 4 * frame.total());
			Detect(fr, targets, nbtargets, maxTargets);
		}
		else if (i > 100)
		{
			memcpy(fr.rawData, frame.data, 4 * frame.total());
			Track(fr, targets, nbtargets);
		}
	}

	//TODO Displace to Close ?
	free(fr.rawData);

	cv::destroyAllWindows();
	cap.release();
	Close();
	std::cout << "End test WorkflowWebcam." << endl;
}
