#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
/*
• Develop a program which edits the input video
• Read ‘video.mp4’. Depending on the input of a user, your program should do the following operations
• Negative transformation on the user input ‘n’ • Gamma transformation with the value of gamma as 2.5 on the user input ‘g’ • Histogram equalization on the user input ‘h’ • Color slicing on the user input ‘s’ • Hue value: 9<hue<23
• Color conversion on the user input ‘c’ • Increase Hue value by 50
• For hue values bigger than 129, subtract 129 instead
• Average filtering on the user input ‘a’ • Use “blur” function with mask size as 9X9
• White balancing by using gray world assumption on the user input ‘w’ • Reset the image on the user input ‘r’

Your program should display one window ‘video’
• Depending on the input of the user, contents in the window should be changed
• For color conversion, use CV_BGR2HSV and CV_HSV2BGR
• Use waitKey for user interaction
• waitKey returns the code of the pressed key or -1 if no key was pressed
before the specified time had elapsed. • ESC  27

*/

int main()
{
    Mat frame;

    int fps;
    int delay, temp;
    int input;
    VideoCapture cap; // 카메라 또는 비디오 파일로 부터 프레임을 읽는데 필요한 정보
    float gamma = 0.5;
    unsigned char pix[256];

    if (cap.open("Road.mp4") == 0)
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }
    fps = cap.get(CAP_PROP_FPS); // 초당 프레임 수

    delay = 1000 / fps;

    while (1)
    {
        cap >> frame;

        if (cap.get(CAP_PROP_POS_MSEC) >= 20000)
        {
            break;
        }

        temp = waitKey(delay);
        cout << temp << endl;

        if (temp != -1)
        {
            input = temp;
        }

        if (input == 110) // n
        {
            for (int j = 0; j < frame.rows; j++)
                for (int i = 0; i < frame.cols * 3; i++)
                {
                    frame.at<uchar>(j, i) = 255 - frame.at<uchar>(j, i);
                }
        }

        if (input == 103)//g
        {
            for (int i = 0; i < 256; i++)
            {
                pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
            }
            for (int j = 0; j < frame.rows; j++)
            {
                for (int i = 0; i < frame.cols * 3; i++)
                {
                    frame.at<uchar>(j, i) = pix[frame.at<uchar>(j, i)];
                }
            }
        }
        if (input ==  104){ //h
            cvtColor(frame, frame, CV_BGR2GRAY);
            equalizeHist(frame, frame); // histogram equlization
        }
        if (input == 115){ //s
        }
        if(input == 99){ //c

        }
        if(input == 97){//a
            blur(frame,frame,Size(9,9));
        }
        if(input == 119){ //w

        }
        if(input == 114){ //r
        
        }
        imshow("video", frame);
    }
}
