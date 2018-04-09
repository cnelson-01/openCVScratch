#include <chrono>
#include <thread>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/imgproc.hpp>


// There are other clocks, but this is usually the one you want.
// It corresponds to CLOCK_MONOTONIC at the syscall level.
using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
//using namespace std::literals::chrono_literals;
using std::this_thread::sleep_for;

using namespace cv;
using namespace std;


int main(int, char **) {
    Mat frame;
    Mat keyFrame;
    Mat res;
    CvFont font;
    cvInitFont(&font, 1, 1, 1);
    double sum = 0;
    uint32_t thresh = 1000000;

    uint32_t numFrames = 10;
    Mat capture[numFrames];
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID + apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
         << "Press any key to terminate" << endl;
    cap.read(keyFrame);
    for (;;) {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        time_point<Clock> start = Clock::now();
        res = keyFrame - frame;
        sum = cv::sum(res)[0];

        if (sum > thresh) {
//            imshow("Live", cv::Scalar(255,255,255) - res);
            imshow("Live", res);
            // Grab a few frames
            for (int i = 0; i < numFrames; i++) {
                cap.read(capture[i]);
            }
            time_point<Clock> end = Clock::now();
            milliseconds diff = duration_cast<milliseconds>(end - start);
            std::cout << diff.count() << "ms" << " " << ((double) numFrames * 1000) / diff.count() << std::endl;
        } else {
            imshow("Live", res);
            time_point<Clock> end = Clock::now();
            milliseconds diff = duration_cast<milliseconds>(end - start);
            std::cout << diff.count() << "ms" << " " << ((double) 1) / diff.count() << std::endl;
        }

        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}