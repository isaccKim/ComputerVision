#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat frame;
    int fps;
    int delay;
    int total;
    VideoCapture cap;

    if (cap.open("background.mp4") == 0)
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }
    fps = cap.get(CAP_PROP_FPS);
    total = cap.get(CAP_PROP_FRAME_COUNT);
    delay = 1000 / fps;
    while (1)
    {
        cap >> frame;
        cout << "frames: " << fps*cap.get(CAP_PROP_POS_MSEC)/1000 << " / " << total << endl;
        if (cap.get(CAP_PROP_POS_MSEC) >=3000)
        {
            break;
        }
        imshow("video", frame);
        waitKey(delay);
    }
}
