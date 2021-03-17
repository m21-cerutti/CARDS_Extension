#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>
#include <Windows.h>
#include "opencv2/video/background_segm.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

#include "videoprovider.h"
#include "video_api.h"
#include "utilities.h"

using namespace cv;
using namespace std;

int main()
{
    int height = 500;
    int width = 500;
    VideoProvider* video = CreateCameraContext(width, height);
    int cpt_frame = 0;
    int saveBGafterFrameNb = 30;
    int threshold_value = 10;
    int max_binary_value = 255;
    char keyboard = 0;
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2(500, 16.0, true);

    Mat img, foregroundMask, backgroundImage;

    while (keyboard != 'q' && keyboard != 27)
    {
        const Frame& _frame = video->GetFrame();
        img = FrameToCVMat(_frame);

        if (foregroundMask.empty()) {
            foregroundMask.create(img.size(), img.type());
        }
        // Compute foreground mask 8 bit image
        // Let time to the camera to open before saving the background and allow object detection
        if (cpt_frame < saveBGafterFrameNb)
        {
            bg_model->apply(img, foregroundMask);
        }
        else
            bg_model->apply(img, foregroundMask, 0);

        GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);
        threshold(foregroundMask, foregroundMask, threshold_value, max_binary_value, THRESH_BINARY);
        bg_model->getBackgroundImage(backgroundImage);

        imshow("foreground mask", foregroundMask);
        imshow("frame", img);
        cpt_frame++;
        keyboard = (char)waitKey(30);
    }
    destroyAllWindows();
    FreeVideoContext(video);
    return 0;
}