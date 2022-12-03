#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat quantization_mat1 = (Mat_<double>(8, 8) << 16, 11, 10, 16, 24, 40, 51, 61,
                         12, 12, 14, 19, 26, 58, 60, 55,
                         14, 13, 16, 24, 40, 57, 69, 56,
                         14, 17, 22, 29, 51, 87, 80, 62,
                         18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99);

Mat quantization_mat2 = (Mat_<double>(8, 8) << 1, 1, 1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

Mat quantization_mat3 = (Mat_<double>(8, 8) << 100, 100, 100, 100, 100, 100, 100, 100,
                         100, 100, 100, 100, 100, 100, 100, 100,
                         100, 100, 100, 100, 100, 100, 100, 100,
                         100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100);

Mat compression(Mat image, Mat matrix)
{
    Mat subimage;
    // Mat result = image.clone();
    float value;
    image.convertTo(image, CV_64FC1);
    // compressed
    for (int j = 0; j < image.cols; j += 8)
    {
        for (int i = 0; i < image.rows; i += 8)
        {
            Rect rect(i, j, 8, 8);
            subimage = image(rect);
            dct(subimage, subimage);
            for (int l = 0; l < 8; l++)
            {
                for (int k = 0; k < 8; k++)
                {

                    subimage.at<double>(l, k) = cvRound(subimage.at<double>(l, k) / matrix.at<double>(l, k));
                    // value = subimage.at<double>(l, k);
                    // cout << value << endl;
                }
            }
            // subimage.convertTo(subimage, CV_8UC1);
            subimage.copyTo(image(rect));
            // merge(subimage,image(rect));
        }
    }

    // imshow("1", image);

    // merge(image, result);

    // decompressed
    for (int j = 0; j < image.cols; j += 8)
    {
        for (int i = 0; i < image.rows; i += 8)
        {
            Rect rect(i, j, 8, 8);
            subimage = image(rect);
            // image(rect).copyTo(subimage);
            for (int l = 0; l < 8; l++)
            {
                for (int k = 0; k < 8; k++)
                {
                    subimage.at<double>(l, k) = cvRound(subimage.at<double>(l, k) * matrix.at<double>(l, k));
                }
            }
            dct(subimage, subimage, 1);
            // multiply(subimage,matrix,subimage);
            subimage.copyTo(image(rect));
            // merge(subimage,image(rect));
        }
    }
    // value = subimage.at<uchar>(7, 7);
    // cout << value << endl;

    // merge(image, result);
    // cvtColor(image, image, CV_YCrCb2BGR);
    image.convertTo(image, CV_8UC1);
    // imshow("1", image);
    return image;
}

double psnr_caculator(Mat input1, Mat input2)
{
    double psnr;
    double mse = 0.0;
    double mn;
    for (int j = 0; j < input1.cols; j++)
    {
        for (int i = 0; i < input1.rows; i++)
        {
            mse = mse + ((input2.at<uchar>(j, i) - input1.at<uchar>(j, i))) * ((input2.at<uchar>(j, i) - input1.at<uchar>(j, i)));
        }
    }
    mn = input1.cols * input1.rows;
    mse = mse / mn;
    cout << mse << endl;
    psnr = (20 * log10(255.0)) - (10 * log10(mse));
    return psnr;
}

int main()
{
    Mat image;
    Mat image_ycbcr;
    Mat ycbcr_channels[3];
    image = imread("lena.png", IMREAD_COLOR);
    int value;
    Mat y(image.rows, image.cols, CV_8UC1);
    Mat result;

    cvtColor(image, image_ycbcr, CV_BGR2YCrCb);

    split(image_ycbcr, ycbcr_channels);

    for (int j = 0; j < image.cols; j++)
    {
        for (int i = 0; i < image.rows; i++)
        {
            y.at<uchar>(j, i) = 0;
            y.at<uchar>(j, i) = ycbcr_channels[0].at<uchar>(j, i);
        }
    }

    Mat r1 = compression(y, quantization_mat1);
    Mat r2 = compression(y, quantization_mat2);
    Mat r3 = compression(y, quantization_mat3);

    // double psnr1 = PSNR(y, r1);
    // double psnr2 = PSNR(y, r2);
    // double psnr3 = PSNR(y, r3);
    double psnr1 = psnr_caculator(y, r1);
    double psnr2 = psnr_caculator(y, r2);
    double psnr3 = psnr_caculator(y, r3);

    cout << fixed;
    cout.precision(6);
    cout << "QM1 : psnr = " << psnr1 << endl;
    cout << "QM2 : psnr = " << psnr2 << endl;
    cout << "QM3 : psnr = " << psnr3 << endl;

    // make_subimage(y, quantization_mat2);
    // make_subimage(y, quantization_mat3);

    imshow("Original Y", y);
    imshow("QM1", r1);
    imshow("QM2", r2);
    imshow("QM3", r3);

    waitKey(0);
    return 0;
}
