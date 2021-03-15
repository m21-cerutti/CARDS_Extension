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
	//TestVideoProvider();
	TestVideoComparison();
}

static void TestWorkflow( VideoProviderConsole& provider );
static void TestComparison(VideoProviderConsole& provider, FileStorage& file_storage);


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

void TestVideoProvider()
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

void TestVideoComparison()
{
	std::cerr << "Opening video test.avi ...\r";

	VideoProviderConsole video("test.avi");
	FileStorage fs2("test.xml", FileStorage::READ);
	TestComparison(video, fs2);

	fs2.release();

	std::cout << "End test VideoComparison." << endl;
}

void TestComparison(VideoProviderConsole& provider, FileStorage& file_storage)
{
	int nbtargets = 0, maxTargets = 5;
	Target targets[5];
	bool isinitialised = false;

	FileNode root = file_storage["Video_annotations"];
	FileNodeIterator it = root.begin(), it_end = root.end();
	int idx = 0;
	double difference_mean = 0;

	for (int i = 0;;)
	{
		const Frame& fr = provider.GetFrame();
		if (fr.rawData == nullptr)
		{
			if (idx > 0)
				std::cout << "Average distance = " << (difference_mean / (double)idx) << endl;
			break;
		}

		if (!isinitialised)
		{
			Mat texture(fr.height, fr.width, CV_8UC4, fr.rawData);
			cvtColor(texture, texture, COLOR_RGBA2BGR);
			cv::imshow("Init", texture);

			if (waitKey(25) == 27)
			{
				Init(targets, nbtargets, maxTargets);
				isinitialised = true;
			}
		}

		else if (isinitialised)
		{
			if (i == 0)
			{
				ManualRegister(fr, targets, nbtargets, maxTargets);
			}
			else
			{
				Track(fr, targets, nbtargets);
			}
			DebugTargets(fr, targets, nbtargets);

			if (it != it_end)
			{
				FileNode xmlframe = (*it)["frame"];
				FileNodeIterator it_frame = xmlframe.begin(), it_frame_end = xmlframe.end();
				int idx_frame = 0;
				int frame_number = (int)(*it)["number"];

				if (frame_number == i)
				{
					for (; it_frame != it_frame_end; ++it_frame, idx_frame++)
					{
						int id = (int)(*it_frame)["id"];
						String state;
						(*it_frame)["state"] >> state;
						bool match = false;

						for (int j = 0; j < maxTargets; j++)
						{
							if (targets[j].id == id)
							{
								if (!match)
								{
									match = true;
								}
								else
								{
									std::cerr << "Id " << id << " found multiple times in target table!" << endl;
									//TODO
								}

								if (!state.compare("Lost"))
								{
									if (targets[j].state == StateTracker::Undefined)
									{
										//TODO
									}
									else
									{
										//TODO
									}
								}
								else if (!state.compare("Live"))
								{
									if (targets[j].state == StateTracker::Undefined)
									{
										//TODO
									}
									else
									{
										double x = (double)(*it_frame)["x"], y = (double)(*it_frame)["y"];
										double width = (double)(*it_frame)["width"], height = (double)(*it_frame)["height"];
										double center_x = x + (width / 2.), center_y = y + (height / 2.);

										double target_center_x = (double)targets[j].rect.x + ((double)targets[j].rect.width / 2.);
										double target_center_y = (double)targets[j].rect.y + ((double)targets[j].rect.height / 2.);

										double norm = sqrt((target_center_x - center_x) * (target_center_x - center_x) +
											(target_center_y - center_y) * (target_center_y - center_y));
										difference_mean += norm;

										std::cout << "Target #" << (idx + 1) << ": euclidian distance = " << norm << endl;
										idx++;
									}
								}
							}
						}

						if (!match)
						{
							std::cerr << "Couldn't find matching id" << endl;
							//TODO
						}

					}
				}
				++it;
			}
			i++;
		}
	}

	cv::destroyAllWindows();
	Close(targets, nbtargets, maxTargets);
}
