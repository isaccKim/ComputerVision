#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat quantization_mat1 = (Mat_<double>(8, 8) << 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99);
Mat quantization_mat2 = (Mat_<double>(8, 8) << 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
Mat quantization_mat3 = (Mat_<double>(8, 8) << 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100);

int width = 512;
int height = 512;

Mat imageshow(Mat mat, Mat y)
{
    Mat block;
    y.convertTo(y, CV_64FC1);

    for (int j = 0; j < height; j += 8)
    {
        for (int i = 0; i < width; i += 8)
        {
            Rect rect(i, j, 8, 8);
            block = y(rect);
            dct(block, block, 0);
            for (int k = 0; k < 8; k++)
            {
                for (int l = 0; l < 8; l++)
                {
                    block.at<double>(k, l) = cvRound(block.at<double>(k, l) / mat.at<double>(k, l));
                }
            }
            block.copyTo(y(rect));
        }
    }

    for (int j = 0; j < 512; j += 8)
    {
        for (int i = 0; i < 512; i += 8)
        {
            Rect rect(i, j, 8, 8);
            block = y(rect);
            for (int k = 0; k < 8; k++)
            {
                for (int l = 0; l < 8; l++)
                {
                    block.at<double>(k, l) = mat.at<double>(k, l) * block.at<double>(k, l);
                }
            }
            dct(block, block, 1);
            block.copyTo(y(rect));
        }
    }
    y.convertTo(y, CV_8UC1);
    return y;
}

double calPSNR(Mat y, Mat temp)
{
    double mse = 0.0;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            mse += (temp.at<uchar>(i, j) - y.at<uchar>(i, j)) * (temp.at<uchar>(i, j) - y.at<uchar>(i, j));
        }
    }
    double max = 255;
    mse = mse / (width * height);
    double psnr = 20 * log10(max) - 10 * log10(mse);

    return psnr;
}

int main()
{
    Mat image;
    Mat image_Ycbcr;
    Mat Ycbcr_channels[3];
    Mat y(512, 512, CV_8UC1);

    image = imread("Lena.png", IMREAD_COLOR);

    int width = image.size().width;
    int height = image.size().height;

    cvtColor(image, image_Ycbcr, CV_BGR2YCrCb);
    split(image_Ycbcr, Ycbcr_channels);

    for (int j = 0; j < 512; j++)
    {
        for (int i = 0; i < 512; i++)
        {
            y.at<uchar>(j, i) = 0;
            y.at<uchar>(j, i) = Ycbcr_channels[0].at<uchar>(j, i);
        }
    }

    Mat temp1 = imageshow(quantization_mat1, y);
    Mat temp2 = imageshow(quantization_mat2, y);
    Mat temp3 = imageshow(quantization_mat3, y);

    imshow("Original Y", image);
    imshow("QM1", temp1);
    imshow("QM2", temp2);
    imshow("QM3", temp3);

    double psnr1 = calPSNR(y, temp1);
    double psnr2 = calPSNR(y, temp2);
    double psnr3 = calPSNR(y, temp3);

    cout << fixed;
    cout.precision(6);

    cout << "QM1: " << psnr1 << endl;
    cout << "QM2: " << psnr2 << endl;
    cout << "QM13 " << psnr3 << endl;

    waitKey(0);
    return 0;
}
