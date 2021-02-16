#include "utilitaries.h"
#include "tracker.h"

using namespace std;
using namespace cv;

Mat TextureToCVMat(const Frame& frame)
{
	Mat texture(frame.height, frame.width, CV_8UC4, frame.rawData);
	cv::cvtColor(texture, texture, cv::COLOR_BGRA2RGB);
	return texture;
}

void DebugMat(const Mat& frame)
{
	cv::imshow("DebugOpenCV", frame);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
