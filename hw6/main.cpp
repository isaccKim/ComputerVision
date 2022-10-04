#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/*
 * Develop a program which performs various kinds of thresholding
 * Use ‘finger_print.png’, ‘adaptive_1.jpg’, ‘adaptive.png’
 * Read all images as gray scale images
 * For ‘finger_print.png’, set finger print region to 0 and background region to 255
 * For ‘adatpve_1.jpg’, and ‘adaptive.png’, set character region to 0 and background region to 255
 * Your program should display three windows'finger_print’, ‘adaptive_1’, ‘adaptive’
 * Each window should display thresholding result
 */

int main()
{
    Mat f_image =imread("finger_print.png");
    Mat a_image =imread("adaptive_1.jpg");
    Mat a1_image =imread("adaptive.png");
    Mat f_dst,a_dst,a1_dst;

    
    cvtColor(f_image,f_image,CV_BGR2GRAY);
    cvtColor(a_image,a_image,CV_BGR2GRAY);
    cvtColor(a1_image,a1_image,CV_BGR2GRAY);

    threshold(f_image,f_dst,160, 255, CV_THRESH_BINARY);
    //inRange(a_image,Scalar(160),Scalar(225),a_dst);
    
    adaptiveThreshold(a_image,a_dst, 255, CV_ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,7,8); //줄무늬 격자가 있는 이미지 => 가우시안블러처리
    
    adaptiveThreshold(a1_image,a1_dst, 255, ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,7,8); //
    

    imshow("finger_print.png",f_dst);
    imshow("adaptive.png",a_dst);
    imshow("adpative1.png",a1_dst);

    waitKey(0);
    return 0;
}
