#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
    Mat image, foregroundMask, backgroundImg, foregroundImg;
    VideoCapture cap("Project2_1.mp4");
    int value;
    int count =0;

    while (true)
    {
        count = 0;
        cap >> image;
        resize(image, image, Size(640, 480));
        if (foregroundMask.empty())
            foregroundMask.create(image.size(), image.type());
        bg_model->apply(image, foregroundMask);
        GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);
        // threshold(foregroundMask, foregroundMask, 180, 255, THRESH_BINARY);
        threshold(foregroundMask, foregroundMask, 180, 255, THRESH_BINARY);
        foregroundImg = Scalar::all(0); 
        image.copyTo(foregroundImg, foregroundMask);
        for(int i=0; i<foregroundMask.rows; i++){
         for(int j=0; j<foregroundMask.cols; j++){
             if(foregroundMask.at<uchar>(i,j) > 0){
                 count++;
             }
        }   
        }
        if(count>1000){
            putText(image,"Alert! Moving object!",Point(20, 60),FONT_HERSHEY_COMPLEX,1,Scalar(0,0,255), 1);
        }
        if (!image.empty())
        {
            imshow("Project2",image);
            imshow("Project3",foregroundMask);
        }
        waitKey(33);
    }
}