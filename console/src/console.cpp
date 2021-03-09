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

static void TestWorkflow( VideoProvider& provider );

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
	int nbtargets = 0,maxTargets = 5;
	Target targets[5];

	//Test DLL
	Init( targets,nbtargets,maxTargets );
	Close( targets,nbtargets,maxTargets );

	std::cout << "End test DLL." << endl;
}

void TestWorkflowVideo()
{
	std::cerr << "Opening video test.mp4 ...\r";

	VideoProvider video( "test.avi" );
	TestWorkflow( video );

	std::cout << "End test WorkflowWebcam." << endl;
}

void TestWorkflowWebcam()
{
	std::cerr << "Opening camera...\r";
	VideoProvider webcam;
	TestWorkflow( webcam );

	std::cout << "End test WorkflowWebcam." << endl;
}

void TestWorkflow( VideoProvider& provider )
{
	int nbtargets = 0,maxTargets = 5;
	Target targets[5];
	bool isinitialised = false;
	//TODO Init ? Automatic calculation ?
	int detect_freq = 20;

	for(int i = 0;;)
	{
		const Frame& fr = provider.GetFrame();
		if(fr.rawData == nullptr)
		{
			break;
		}

		if(!isinitialised)
		{
			Mat texture( fr.height,fr.width,CV_8UC4,fr.rawData );
			cvtColor( texture,texture,COLOR_RGBA2BGR );
			cv::imshow( "Init",texture );

			if(waitKey( 25 ) == 27)
			{
				Init( targets,nbtargets,maxTargets );
				isinitialised = true;
			}
		}
		else if(isinitialised)
		{
			if(i == 0)
			{
				ManualRegister( fr,targets,nbtargets,maxTargets );
				//Detect( fr,zoneDetection,targets,nbTarget,maxTarget );
			}
			else if(i % detect_freq == 0)
			{
				//std::cout << "Detect" << endl;
				//Detect( fr,zoneDetection,targets,nbTarget,maxTarget );
			}
			else
			{
				Track( fr,targets,nbtargets );
			}
			DebugTargets( fr,targets,nbtargets );
			i++;
		}
	}

	cv::destroyAllWindows();
	Close( targets,nbtargets,maxTargets );
}
