#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
 
int main()
{
    Mat image_m,GaussianImg,image_result1,image_result2;
    image_m = imread("Moon.png", 0);
    //image_result2 = imread("Moon.png", 0);
    image_result2 = image_m.clone();
    Rect rect_1(image_m.cols/2,0,image_m.cols/2,image_m.rows); 


    imshow("moon", image_m);
    GaussianBlur(image_m, GaussianImg, Size(3, 3), 0, 0, BORDER_DEFAULT);
    image_result1 = image_m-GaussianImg; //gmask = 일반 이미지 - 가우시안 블러 처한 이지
    add(image_m(rect_1),image_result1(rect_1)*6,image_result2(rect_1));

   imshow("moon_filtered", image_result2);
    
    Mat image_s = imread("saltnpepper.png",0);
    
    imshow("saltnpepper",image_s);
    Rect rect_2(0,0,image_s.cols/2,image_s.rows); // 지금 되는거 
    medianBlur(image_s(rect_2),image_s(rect_2),9);        
    imshow("saltnpepper_filtered",image_s);

    waitKey(0);
}



