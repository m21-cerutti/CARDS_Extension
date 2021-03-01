#include "console.h"
#include "videoprovider.h"

using namespace std;
using namespace cv;

int main( int argc,char** argv )
{
	TestDLL();
	//TestWebcam();
	TestWorkflowVideo();
	//TestWorkflowWebcam();
}

void TestWebcam()
{
	std::cerr << "Opening camera...\r";
	//TODO Need to accelerate opening
	VideoCapture cap;
	if(!cap.open( 0,cv::CAP_ANY ))
	{
		std::cerr << "Test camera failed ! Can't open." << std::endl;
		return;
	}

	for(int i = 0; i < 200; i++)
	{
		Mat frame;
		cap >> frame;
		if(frame.empty())
		{
			std::cerr << "Warning! Empty frame." << std::endl;
			break;
		}
		imshow( "Test webcam",frame );
		if(waitKey( 10 ) == 27) break; // stop capturing by pressing ESC
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

void TestWorkflowVideo()
{
	std::cerr << "Opening video test.mp4 ...\r";

	VideoProvider video( "test.avi" );

	int nbtargets = 0,maxTargets = 5;
	Target targets[5];
	bool isinitialised = false;

	for(int i = -1;;)
	{
		const Frame& fr = video.GetFrame();
		if(fr.rawData == nullptr)
		{
			break;
		}

		if(!isinitialised)
		{
			Mat texture( fr.height,fr.width,CV_8UC4,fr.rawData );
			cvtColor( texture,texture,COLOR_RGBA2BGR );
			cv::imshow( "Init",texture );

			if(waitKey( 10 ) == 27) isinitialised = true;
		}
		else if(isinitialised && i < 0)
		{
			Init();
		}
		else if(i < 1)
		{
			Detect( fr,targets,nbtargets,maxTargets );
		}
		else if(i > 0)
		{
			Track( fr,targets,nbtargets );
		}

		if(isinitialised)
		{
			i++;
		}
	}

	cv::destroyAllWindows();
	Close();
	std::cout << "End test WorkflowWebcam." << endl;
}

void TestWorkflowWebcam()
{
	std::cerr << "Opening camera...\r";
	VideoProvider video;

	int nbtargets = 0,maxTargets = 5;
	Target targets[5];

	for(int i = 0;; i++)
	{
		const Frame& fr = video.GetFrame();
		if(fr.rawData == nullptr)
		{
			break;
		}

		if(i == 50)
		{
			Init();

			//Debug
			Mat texture( fr.height,fr.width,CV_8UC4,fr.rawData );
			cvtColor( texture,texture,COLOR_RGBA2BGR );
			cv::imshow( "Init",texture );
		}
		else if(i == 100)
		{
			Detect( fr,targets,nbtargets,maxTargets );
		}
		else if(i > 100)
		{
			Track( fr,targets,nbtargets );
		}
	}

	cv::destroyAllWindows();
	Close();
	std::cout << "End test WorkflowWebcam." << endl;
}
