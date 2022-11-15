#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
    Mat image, foregroundMask, backgroundImg, foregroundImg;
    VideoCapture cap("Project2.mp4");
    // VideoCapture cap("background.mp4");
    // VideoCapture cap("background.mp4");
    int value;
    int count =0;

    while (true)
    {
        count = 0;
        cap >> image;
        resize(image, image, Size(640, 480));
        if (foregroundMask.empty())
            foregroundMask.create(image.size(), image.type());
        // image: Next video frame. Floating point frame will be used without scaling and //should be in range [0,255]. // foregroundMask: The output foreground mask as an 8-bit binary image.
        bg_model->apply(image, foregroundMask);
        GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);
        threshold(foregroundMask, foregroundMask, 180, 255, THRESH_BINARY);
        foregroundImg = Scalar::all(0);
        image.copyTo(foregroundImg, foregroundMask);
        // backgroundImg: The output background image. bg_model->getBackgroundImage(backgroundImg);
        
        for(int i=0; i<foregroundMask.rows; i++){
         for(int j=0; j<foregroundMask.cols; j++){
             if(foregroundMask.at<uchar>(i,j) > 0){
                 count++;
             }
        }   
        }
        
        cout << count << endl;

        if(count>1000){
            putText(image,"Alert! Moving object!",Point(20, 60),FONT_HERSHEY_COMPLEX,1,Scalar(0,0,255), 1);
        }

        imshow("foreground mask", foregroundMask);
        imshow("image",image);
        imshow("foreground image", foregroundImg);
        if (!backgroundImg.empty())
        {
            imshow("mean background image", backgroundImg);
        }
        waitKey(33);
    }
}