#include "console.h"
#include "videoproviderconsole.h"

using namespace std;
using namespace cv;

//TODO tests api Google ?
int main( int argc,char** argv )
{
	TestDLL();
	//TestWorkflowVideo();
	TestWorkflowWebcam();
	//TestVideoContext();
	//WriteXML();
}

static void TestWorkflow( VideoProviderConsole& provider );

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
	std::cerr << "Opening test video ..." << endl;

	VideoProviderConsole video( "test2.mp4" );
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
	PoseParameters pose_params = PoseParameters();
	GetPoseParameters( ".",pose_params );

	int nbtargets = 0,maxTargets = 5;
	Target targets[5];
	bool isinitialised = false;
	bool zoneDetected = false;
	Frame frbg;
	RectStruct zoneDetection;

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
			// Press s to save the background
			if (waitKey(25) == 's')
			{
				zoneDetected = true;
				const Frame &frBackground = provider.CopyFrame();
				frbg = frBackground;
				zoneDetection = { (float)(frbg.width * 0.05),
					(float)(frbg.height * 0.55),
					(float)(frbg.width * 0.5 - frbg.width * 0.1),
					(float)(frbg.height * 0.9 - frbg.height * 0.5) };
				cout << "Detect zone in the bottom left corner, wait until the object is detected then move it away." << endl;
				
			}
			if (zoneDetected == true)
			{
				if (i % detect_freq == 0)
					Detect(fr, frbg, zoneDetection, targets, nbtargets, maxTargets);
			}

			/*if (i == 0)
			{
				ManualRegister(fr, targets, nbtargets, maxTargets);
			}*/
			if (i % detect_freq == 0)
			{
				//std::cout << "CheckTrack" << endl;
				//CheckTrack( fr,zoneDetection,targets,nbTarget,maxTarget );
			}
			else
			{
				Track(fr, targets, nbtargets);
				if (targets[0].state != StateTracker::Undefined)
				{
					Matrix4x4f matpos = EstimatePose(targets[0], pose_params);
					//cout << matpos.c_03 << endl; // X
					//cout << matpos.c_13 << endl; // Y
					cout << matpos.c_23 << endl; // Z
				}
			}
			DebugTargets(fr, targets, nbtargets);

			i++;
			if (waitKey(25) == 'q')
				break;
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

void WriteXML()
{
	std::cerr << "Opening video test.avi ...\r";

	VideoProviderConsole video( "test.avi" );
	FileStorage fs( "test.xml",FileStorage::WRITE );

	int freq_record = 20;
	int maxTargets = 5;
	int ids_to_use[5];
	for(int i = 0; i < maxTargets; i++)
	{
		ids_to_use[i] = i;
	}
	bool isinitialised = false;

	fs << "Frames" << "{";

	for(int i = 0;;)
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
			isinitialised = true;
		}
		else if(isinitialised)
		{
			if(i % freq_record == 0)
			{
				fs << "Frame";
				fs << "{" << "number" << i << "ListTargets" << "{";

				Mat texture( fr.height,fr.width,CV_8UC4,fr.rawData );
				cvtColor( texture,texture,cv::COLOR_RGBA2BGR );
				flip( texture,texture,0 );
				int flip_code = (int)fr.flip_mode - 2;
				if(flip_code >= -1)
				{
					flip( texture,texture,flip_code );
				}

				for(int j = 0; j < maxTargets; j++)
				{
					cout << "Id " << j << endl;
					Rect roi = selectROI( "select",texture );
					fs << "XMLTarget";
					if((roi.width == 0) && (roi.height == 0))
					{
						fs << "{" << "id" << ids_to_use[j]
							<< "state" << "Lost" << "}";
					}
					else
					{
						fs << "{" << "id" << ids_to_use[j]
							<< "state" << "Live"
							<< "x" << roi.x
							<< "y" << roi.y
							<< "width" << roi.width
							<< "height" << roi.height << "}";
					}
				}
				fs << "}" << "}";
			}
			i++;
		}
	}
	fs << "}";
	fs.release();

	std::cout << "End test WriteXML." << endl;
}
