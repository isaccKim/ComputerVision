#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat frame,left, right;
    int fps;
    int delay;
    int total;
    VideoCapture cap; // 카메라 또는 비디오 파일로 부터 프레임을 읽는데 필요한 정보
    Rect left_roi(200,400,600,600);
    Rect right_roi(600,400,1000,600);

    if (cap.open("Road.mp4") == 0)
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }
    fps = cap.get(CAP_PROP_FPS); // 초당 프레임 수

    right = frame(right_roi);
    //delay = 1000 / fps;
    while (1)
    {
        cap >> frame; 
        if(frame.empty()){
            cout << "end of viedo" << endl;
            break;
        }
        imshow("video", frame);
        imshow("left", left);
        imshow("right", right);
        waitKey(delay);
    }
}
