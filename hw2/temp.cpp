#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image = imread("lena.png", 0);
    Mat r_image = image.clone();

    unsigned char pix[256];
    

    for (int j = 0; j < image.rows; j++)
    {
        for (int i = 0; i < image.cols; i++)
        {
            // r_image.at<uchar>(image.rows-j-1, i) = image.at<uchar>(j, i); // 위아래
            r_image.at<uchar>(i,j) = image.at<uchar>(j, i); // 반시게
            // r_image.at<uchar>(i,image.rows-1-j) = image.at<uchar>(j, i); // 시계 
        }
    }
    imshow("Input image", image);
    imshow("Negative transformation", r_image);
    waitKey(0);

    return 0;
}
