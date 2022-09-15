#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
 
int main()
{
    Mat image_m, laplacian, abs_laplacian, sharpening;
    image_m = imread("Moon.png", 0);
    imshow("moon", image_m);

    GaussianBlur(image_m, image_m, Size(3, 3), 0, 0, BORDER_DEFAULT);
    Laplacian(image_m, laplacian, CV_16S, 1, 1, 0);
    convertScaleAbs(laplacian, abs_laplacian);
    sharpening = abs_laplacian+ image_m;
    
      for(int i=image_m.cols/2; i<image_m.cols; i++){
        for(int j=0; j<image_m.rows; j++){
            image_m.at<uchar>(j, i) = sharpening.at<uchar>(j,i);
        }
    }
    
    imshow("moon_filtered", image_m);
    
    Mat image_s = imread("saltnpepper.png",0);
    Mat  image_sf= imread("saltnpepper.png",0);
    
    imshow("saltnpepper",image_s);

    medianBlur(image_s,image_s,9);        
    for(int i=0; i<image_s.cols/2; i++){
        for(int j=0; j<image_s.rows; j++){
            image_sf.at<uchar>(j, i) = image_s.at<uchar>(j,i);
        }
    }
    imshow("saltnpepper_filtered",image_sf);

    waitKey(0);
}


