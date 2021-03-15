#include "console.h"
#include "videoproviderconsole.h"

using namespace std;
using namespace cv;

int main( int argc,char** argv )
{
	TestDLL();
	//TestWebcam();
	//TestWorkflowVideo();
	//TestWorkflowWebcam();
	TestVideoContext();
}

static void TestWorkflow( VideoProviderConsole& provider );

void TestWebcam()
{
	std::cerr << "Opening camera..." << endl;
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
	std::cerr << "Opening video test.avi ..." << endl;

	VideoProviderConsole video( "test.avi" );
	TestWorkflow( video );

	std::cout << "End test WorkflowWebcam." << endl;
}

void TestWorkflowWebcam()
{
	std::cerr << "Opening camera...\r";
	VideoProviderConsole webcam;
	TestWorkflow( webcam );

	std::cout << "End test WorkflowWebcam." << endl;
}

void TestWorkflow( VideoProviderConsole& provider )
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
			//Detect( fr,zoneDetection,targets,nbTarget,maxTarget );

			if(i == 0)
			{
				ManualRegister( fr,targets,nbtargets,maxTargets );
			}
			else if(i % detect_freq == 0)
			{
				//std::cout << "CheckTrack" << endl;
				//CheckTrack( fr,zoneDetection,targets,nbTarget,maxTarget );
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

void TestVideoContext()
{
	std::cerr << "Opening video...\r";

	VideoProvider* video = CreateVideoContext( "./test.avi",256,256 );


	for(int i = 0;; i++)
	{
		Frame fr;
		GetFrame( video,fr );
		if(fr.rawData == nullptr)
		{
			break;
		}

		Mat texture( fr.height,fr.width,CV_8UC4,fr.rawData );
		cvtColor( texture,texture,COLOR_RGBA2BGR );

		imshow( "Test webcam",texture );
		if(waitKey( 25 ) == 27) break; // stop capturing by pressing ESC
	}
	cv::destroyAllWindows();
	FreeVideoContext( video );

	std::cout << "End test VideoProvider." << endl;
}