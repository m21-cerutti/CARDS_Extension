#include "console.h"
#include "videoproviderconsole.h"

using namespace std;
using namespace cv;

//TODO tests api Google ?
int main( int argc,char** argv )
{
	TestDLL();
	//TestWorkflowVideo();
	//TestWorkflowWebcam();
	//TestVideoContext();
	TestWriteXML();
	//TestVideoComparison();
}

static void TestWorkflow( VideoProviderConsole& provider );
static void TestComparison( VideoProviderConsole& provider,FileStorage& file_storage );

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

void TestVideoComparison()
{
	std::cerr << "Opening video test.avi ...\r";

	VideoProviderConsole video( "test.avi" );
	FileStorage fs2( "test.xml",FileStorage::READ );
	TestComparison( video,fs2 );

	fs2.release();

	std::cout << "End test VideoComparison." << endl;
}

void TestWriteXML()
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

void TestComparison( VideoProviderConsole& provider,FileStorage& file_storage )
{
	int nbtargets = 0,maxTargets = 5;
	Target targets[5];
	bool isinitialised = false;

	FileNode root = file_storage["frame"];
	FileNodeIterator it = root.begin(),it_end = root.end();
	int idx = 0;
	double difference_mean = 0;
	int num_frame = 0;

	std::ofstream csv;
	csv.open( "test.csv" );
	csv << "Frame number,ID,Target state,Anotation state,Euclidian norm\n";

	for(int i = 0;;)
	{
		const Frame& fr = provider.GetFrame();
		if(fr.rawData == nullptr)
		{
			if(idx > 0)
				std::cout << "Average distance = " << (difference_mean / (double)idx) << endl;
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
			}
			else
			{
				Track( fr,targets,nbtargets );
			}
			DebugTargets( fr,targets,nbtargets );

			if(it != it_end)
			{
				int frame_number = (int)(*it)["number"];
				while(frame_number < num_frame)
				{
					csv << frame_number << ",Skipped,Skipped,Skipped,Skipped\n";
					++it;
					frame_number = (int)(*it)["number"];
				}

				FileNode xmlframe = (*it)["Targets"];
				FileNodeIterator it_frame = xmlframe.begin(),it_frame_end = xmlframe.end();
				int idx_frame = 0;
				//std::cout << "Frame " << num_frame << ", Frame number " << frame_number << endl;

				if(frame_number == num_frame)
				{
					for(; it_frame != it_frame_end; ++it_frame,idx_frame++)
					{
						int id = (int)(*it_frame)["id"];
						String state;
						(*it_frame)["state"] >> state;
						bool match = false;
						bool duplicate = false;

						for(int j = 0; j < maxTargets; j++)
						{
							if((targets[j].id == id) && (targets[j].state != StateTracker::Undefined))
							{
								csv << frame_number << "," << id << ",";

								if(!match)
								{
									match = true;
								}
								else
								{
									duplicate = true;
									std::cerr << "Id " << id << " found multiple times in target table!" << endl;
									csv << "Duplicate" << "," << state << ",";
								}

								if((targets[j].state != StateTracker::Lost) && (targets[j].state != StateTracker::Live))
								{
									csv << "Unknown" << "," << state << "," << "NaN\n";
								}

								if(!state.compare( "Lost" ))
								{
									if(targets[j].state == StateTracker::Lost)
									{
										csv << "Lost" << "," << state << "," << "0\n";
									}
									else if(targets[j].state == StateTracker::Live)
									{
										csv << "Live" << "," << state << "," << "Infinity\n";
									}
									else
									{
										csv << "Unknown" << "," << state << "," << "NaN\n";
									}
								}
								else if(!state.compare( "Live" ))
								{
									if(!duplicate)
									{
										if(targets[j].state == StateTracker::Lost)
										{
											csv << "Lost" << "," << state << ",";
										}
										else if(targets[j].state == StateTracker::Live)
										{
											csv << "Live" << "," << state << ",";
										}
									}

									double x = (double)(*it_frame)["x"],y = (double)(*it_frame)["y"];
									double width = (double)(*it_frame)["width"],height = (double)(*it_frame)["height"];
									double center_x = x + (width / 2.),center_y = y + (height / 2.);

									double target_center_x = (double)targets[j].rect.x + ((double)targets[j].rect.width / 2.);
									double target_center_y = (double)targets[j].rect.y + ((double)targets[j].rect.height / 2.);

									double norm = sqrt( (target_center_x - center_x) * (target_center_x - center_x) +
														(target_center_y - center_y) * (target_center_y - center_y) );
									difference_mean += norm;

									std::cout << "Target #" << (idx + 1) << ": euclidian distance = " << norm << endl;
									csv << norm << "\n";
									idx++;
								}
							}
						}

						if(!match)
						{
							std::cerr << "Couldn't find matching id" << endl;
							csv << frame_number << "," << id << "," << "Not found" << "," << state << "," << "NaN\n";
						}

					}
					++it;
				}
			}
			i++;
		}
		num_frame++;
	}

	cv::destroyAllWindows();
	Close( targets,nbtargets,maxTargets );

	csv.close();
}
