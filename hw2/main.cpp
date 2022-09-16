#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{ 
    Mat image = imread("lena.png", 0);
    Mat negative_img = image.clone();

    unsigned char pix[256];
    float gamma = 10.0;

    for (int i = 0; i < 256; i++)
    {
        pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
    }
    for (int j = 0; j < image.rows; j++)
        for (int i = 0; i < image.cols; i++)
        {
            if (negative_img.at<uchar>(j, i) < 127)
            {
                negative_img.at<uchar>(j, i) = 255 - image.at<uchar>(j, i);
            }
            else
            {
                negative_img.at<uchar>(j, i) = pix[negative_img.at<uchar>(j, i)];
            }
        }
    imshow("Input image", image);
    rotate(negative_img, negative_img, ROTATE_90_COUNTERCLOCKWISE);
    imshow("Negative transformation", negative_img);
    waitKey(0);

    return 0;
}
