#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
/*
• Develop a program which edits the input video
• Read ‘video.mp4’. Depending on the input of a user, your program should do the following operations
• Negative transformation on the user input ‘n’
• Gamma transformation with the value of gamma as 2.5 on the user input ‘g’
• Histogram equalization on the user input ‘h’
• Color slicing on the user input ‘s’
    • Hue value: 9<hue<23
• Color conversion on the user input ‘c’
    • Increase Hue value by 50
    • For hue values bigger than 129, subtract 129 instead
• Average filtering on the user input ‘a’
• Use “blur” function with mask size as 9X9
• White balancing by using gray world assumption on the user input ‘w’
• Reset the image on the user input ‘r’

Your program should display one window ‘video’
• Depending on the input of the user, contents in the window should be changed
• For color conversion, use CV_BGR2HSV and CV_HSV2BGR
• Use waitKey for user interaction
• waitKey returns the code of the pressed key or -1 if no key was pressed
before the specified time had elapsed.
• ESC  27

*/

int main()
{
    Mat frame;

    int fps;
    int delay, temp;
    int input = 114;
    VideoCapture cap; // 카메라 또는 비디오 파일로 부터 프레임을 읽는데 필요한 정보
    float gamma = 2.5;
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

        temp = waitKey(delay);

        if (temp != -1)
        {
            if (temp == 110 || temp == 103 || temp == 104 || temp == 115 || temp == 99 || temp == 97 || temp == 119 || temp == 114)
            {
                input = temp;
            }
        }

        if (input == 110) // n : negative
        {
            Mat temp;
            cvtColor(frame, temp, CV_BGR2HSV);
            vector<Mat> channels;
            split(temp, channels);

            for (int j = 0; j < frame.cols; j++)
                for (int i = 0; i < frame.rows; i++)
                {
                    channels[2].ptr<uchar>(i, j)[0] = 255 - channels[2].ptr<uchar>(i, j)[0];
                    channels[2].ptr<uchar>(i, j)[1] = 255 - channels[2].ptr<uchar>(i, j)[1];
                    channels[2].ptr<uchar>(i, j)[2] = 255 - channels[2].ptr<uchar>(i, j)[2];
                }

            merge(channels, temp);
            cvtColor(temp, temp, CV_HSV2BGR);
            frame = temp;

            imshow("video", frame);
        }

        else if (input == 103) // gamma
        {
            Mat temp;
            cvtColor(frame, temp, CV_BGR2HSV);
            vector<Mat> channels;
            split(temp, channels);

            for (int i = 0; i < 256; i++)
            {
                pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
            }

            for (int j = 0; j < frame.cols; j++)
                for (int i = 0; i < frame.rows; i++)
                {
                    // negative_img.at<uchar>(j, i) = pix[negative_img.at<uchar>(j, i)];
                    channels[2].ptr<uchar>(i, j)[0] = pix[channels[2].ptr<uchar>(i, j)[0]];
                    channels[2].ptr<uchar>(i, j)[1] = pix[channels[2].ptr<uchar>(i, j)[1]];
                    channels[2].ptr<uchar>(i, j)[2] = pix[channels[2].ptr<uchar>(i, j)[2]];
                }

            merge(channels, temp);
            cvtColor(temp, temp, CV_HSV2BGR);
            frame = temp;
            imshow("video", frame);
        }
        else if (input == 104)
        { // histogram
            Mat temp;
            cvtColor(frame, temp, CV_BGR2HSV);
            vector<Mat> channels;
            split(temp, channels);
            equalizeHist(channels[2], channels[2]);
            merge(channels, temp);
            cvtColor(temp, temp, CV_HSV2BGR);
            frame = temp;
            imshow("video", frame);
        }
        else if (input == 115)
        { // slice
             Mat temp;
            cvtColor(frame, temp, CV_BGR2HSV);
            vector<Mat> channels;
            split(temp, channels);

            for(int j=0; j<frame.cols; j++){
                for(int i=0; i<frame.rows; i++){
                    if(channels[2].ptr<uchar>(i, j)[0]>9 && channels[2].ptr<uchar>(i, j)[0]<11){
                        channels[2].ptr<uchar>(i, j)[0] = channels[2].ptr<uchar>(i, j)[0];
                    }
                    else {
                        channels[2].ptr<uchar>(i, j)[0] = 0;
                    }
                }
            }
            merge(channels, temp);
            
            cvtColor(temp, temp, CV_HSV2BGR);
            frame = temp;
            imshow("video", frame);
        }
        else if (input == 99)
        { // c
        imshow("video", frame);
        }
        else if (input == 97)
        { // a verage
            blur(frame, frame, Size(9, 9));
            imshow("video", frame);
        }
        else if (input == 119)
        { // w hite balance
            Mat bgr_channels[3];
            split(frame, bgr_channels);
            double avg;
            int sum, temp, i, j, c;
            for (c = 0; c < frame.channels(); c++)
            {
                sum = 0;
                avg = 0.0f;
                for (i = 0; i < frame.rows; i++)
                {
                    for (j = 0; j < frame.cols; j++)
                    {
                        sum += bgr_channels[c].at<uchar>(i, j);
                    }
                }
                avg = sum / (frame.rows * frame.cols);
                for (i = 0; i < frame.rows; i++)
                {
                    for (j = 0; j < frame.cols; j++)
                    {
                        temp = (128 / avg) * bgr_channels[c].at<uchar>(i, j);
                        if (temp > 255)
                            bgr_channels[c].at<uchar>(i, j) = 255;
                        else
                            bgr_channels[c].at<uchar>(i, j) = temp;
                    }
                }
            }
            merge(bgr_channels, 3, frame);
            imshow("video", frame);
        }
        else if (input == 114)
        {
            imshow("video", frame);
        }
        else
            continue;
    }
}
