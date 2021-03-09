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
#include "object_detection.h"

using namespace cv;
using namespace std;



int main(int argc, char* argv[])
{
    objectDetection();
    return EXIT_SUCCESS;
}

void objectDetection()
{
    char keyboard;
    // Initialize background substractor
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2(500, 16.0, true);

    Mat img, foregroundMask, backgroundImage;
    VideoCapture cap(0);
    int cpt_frame = 0;
    keyboard = 0;

    // q or ESC to end
    while (keyboard != 'q' && keyboard != 27)
    {
        bool ok = cap.grab();
        if (ok == false) {
            std::cout << "Video Capture Fail" << std::endl;
        }
        else {
            if (!cap.read(img)) {
                cerr << "Unable to read next frame." << endl;
                cerr << "Exiting..." << endl;
                exit(EXIT_FAILURE);
            }

            if (foregroundMask.empty()) {
                foregroundMask.create(img.size(), img.type());
            }
            // Compute foreground mask 8 bit image
            // Let time to the camera to open before not updating the background and allow object detection
            if (cpt_frame > 50)
                bg_model->apply(img, foregroundMask, 0);
            else
                bg_model->apply(img, foregroundMask);

            // smooth the mask to reduce noise
            // todo : testing erode and dilate to clean mask
            GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);

            // threshold mask to saturate at black and white values
            //threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY);

            //Get background image
            bg_model->getBackgroundImage(backgroundImage);
            imshow("foreground mask", foregroundMask);
            imshow("frame", img);
            cpt_frame++;
            keyboard = (char)waitKey(30);
            
        }
    }
    //delete capture object
    cap.release();
    //destroy GUI windows
    destroyAllWindows();
}
