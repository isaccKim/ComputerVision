#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat drawHistogram(Mat src)
{
    Mat hist,histImage;// establish the number of bins
    int i, hist_w, hist_h, bin_w, histSize;
    float range[] = {0, 255};
    const float *histRange = {range};
    hist_w = 512;
    hist_h = 512;
    histSize = 16;
    bin_w = cvRound((double)hist_w / histSize);
    histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
    calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    for (i = 0; i < histSize; i++)
    {
        rectangle(histImage, Point(bin_w * i, hist_h), Point(bin_w * i + hist_w / histSize, hist_h - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), -1);
    }

    return histImage;
}

int main()
{
    Mat image;
    Mat hist_equalized_image;
    Mat hist_graph;
    Mat hist_equalized_graph;
    image = imread("Moon.png", 0);
    if (!image.data)
        exit(1);                               // Check image
    equalizeHist(image, hist_equalized_image); // histogram equlization
    hist_graph = drawHistogram(image);

    hist_equalized_graph = drawHistogram(hist_equalized_image);

for(int i=0; i<8; i++){
            putText(image,format("bin %d : %d",i+1,1),Point(40,40+i*40),FONT_HERSHEY_COMPLEX,0.5,Scalar(0,200,200),1);
    }
    imshow("before", image);

    imshow("after",
           hist_equalized_image);

    imshow("h1",
           hist_graph);

    imshow("h2",
           hist_equalized_graph);

   
    waitKey(0);
    return 0;
}


