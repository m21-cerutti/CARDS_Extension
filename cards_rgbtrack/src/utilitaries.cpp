#include "utilitaries.h"
#include "tracker.h"

using namespace std;
using namespace cv;

Mat TextureToCVMat(const Frame& frame)
{
	// TODO Need some checks (frame initialised)
	Mat texture(frame.height, frame.width, CV_8UC4, frame.rawData);
	cv::cvtColor(texture, texture, cv::COLOR_RGBA2BGR);
	return texture;
}

void DebugMat(const Mat& mat)
{
	cv::imshow("DebugOpenCV", mat);
	cv::waitKey(25);
}

void DebugTargets(const Mat& mat, const std::vector<Rect2d>& objects, int number)
{
	Mat img = mat.clone();
	for (unsigned i = 0; i < number; i++)
		cv::rectangle(img, objects[i], Scalar(255, 0, 0), 2, cv::LINE_8);
	DebugMat(img);
}

Ptr<Tracker> createTrackerByName(const std::string& name)
{
	using namespace cv;

	cv::Ptr<cv::Tracker> tracker;

	if (name == "KCF")
		tracker = TrackerKCF::create();
	else if (name == "TLD")
		tracker = TrackerTLD::create();
	else if (name == "BOOSTING")
		tracker = TrackerBoosting::create();
	else if (name == "MEDIAN_FLOW")
		tracker = TrackerMedianFlow::create();
	else if (name == "MIL")
		tracker = TrackerMIL::create();
	else if (name == "GOTURN")
		tracker = TrackerGOTURN::create();
	else if (name == "MOSSE")
		tracker = TrackerMOSSE::create();
	else if (name == "CSRT")
		tracker = TrackerCSRT::create();
	else
		CV_Error(cv::Error::StsBadArg, "Invalid tracking algorithm name\n");

	return tracker;
}
