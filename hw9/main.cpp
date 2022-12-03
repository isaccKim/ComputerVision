#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image;
    Mat image_Ycbcr;
    Mat Ycbcr_channels[3];
    Mat temp;
    Mat y(512, 512, CV_8UC1);
    image = imread("Lena.png", IMREAD_COLOR);
    Mat quantization_mat1 = (Mat_<double>(8, 8) <<16, 11, 10, 16, 24, 40, 51, 61,12, 12, 14, 19, 26, 58, 60, 55,14, 13, 16, 24, 40, 57, 69, 56,14, 17, 22, 29, 51, 87, 80, 62,18, 22, 37, 56, 68, 109, 103, 77,24, 35, 55, 64, 81, 104, 113, 92,49, 64, 78, 87, 103, 121, 120, 101,72, 92, 95, 98, 112, 100, 103, 99);
    Mat quantization_mat2 = (Mat_<double>(8, 8) <<1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1,1, 1, 1, 1, 1, 1, 1, 1);
    Mat quantization_mat3 = (Mat_<double>(8, 8) <<100, 100, 100, 100, 100, 100, 100, 100,100, 100, 100, 100, 100, 100, 100, 100,100, 100, 100, 100, 100, 100, 100, 100,100, 100, 100, 100, 100, 100, 100, 100,100, 100, 100, 100, 100, 100, 100, 100,100, 100, 100, 100, 100, 100, 100, 100,100, 100, 100, 100, 100, 100, 100, 100,100, 100, 100, 100, 100, 100, 100, 100);
    int height = image.size().height;
    int width = image.size().width;
    Mat finalImage;
   
    cvtColor(image, image_Ycbcr, CV_BGR2YCrCb);
    split(image_Ycbcr, Ycbcr_channels);
    
    for (int j = 0; j < 512; j++)
    {
        for (int i = 0; i < 512; i++)
        {
            y.at<uchar>(j, i) = 0;
            y.at<uchar>(j, i) = Ycbcr_channels[0].at<uchar>(j, i);
            cout << y.at<uchar>(j, i) << endl;
        }
    }

    vector<Mat> planes;
    split(image_Ycbcr, planes);

    resize(planes[1], planes[1], Size(width/2, height/2));
    resize(planes[2], planes[2], Size(width/2, height/2));

    resize(planes[1], planes[1], Size(width, height));
    resize(planes[2], planes[2], Size(width, height));

    Mat mat1 = Mat(8,8,CV_64FC1,&quantization_mat1);
    Mat mat2 = Mat(8,8,CV_64FC1,&quantization_mat2);
    Mat mat3 = Mat(8,8,CV_64FC1,&quantization_mat3);

    cout << "Mat1" << mat1 << endl;
    cout << "Mat2" << mat2 << endl;
    cout << "Mat3" << mat2 << endl;

    vector<Mat> planes;
    split(image_Ycbcr, planes);

// Downsampling chrominance
// Resizing to 1/4 of original image
resize(planes[1], planes[1], Size(width/2, height/2));
resize(planes[2], planes[2], Size(width/2, height/2));

// Resizing to original image size
resize(planes[1], planes[1], Size(width, height));
resize(planes[2], planes[2], Size(width, height));

// Dividing image in blocks 8x8
for ( int i = 0; i < height; i+=8 ){
    for( int j = 0; j < width; j+=8 ){
        // For each plane
        for( int plane = 0; plane < image_Ycbcr.channels(); plane++ ){

            // Creating a block
            Mat block = planes[plane](Rect(j, i, 8, 8));

            // Converting the block to float
            block.convertTo( block, CV_64FC1 );

            // Subtracting the block by 128
            subtract( block, 128.0, block );

            // DCT
            dct( block, block );

            // Applying quantization
            if( plane == 0 ){
                divide( block,quantization_mat1, block );
            }
            else {
                divide( block, quantization_mat2, block );
            }

            // Converting it back to unsigned int
            block.convertTo( block, CV_8UC1 );

            // Copying the block to the original image
            block.copyTo( planes[plane](Rect(j, i, 8, 8)) );
        }
    }
}

    merge( planes, finalImage );

    int height = finalImage.size().height;
int width = finalImage.size().width;

// Transforming 2D Array in Image Matrix
Mat luminance = Mat(8,8, CV_64FC1, &mat1);
Mat chrominance = Mat(8,8, CV_64FC1, &mat2);

// Splitting the image into 3 planes
vector<Mat> planes;
split(finalImage, planes);

// Dividing the image in blocks 8x8
for ( int i = 0; i < height; i+=8 ){
    for( int j = 0; j < width; j+=8 ){
        // For each plane
        for( int plane = 0; plane < finalImage.channels(); plane++ ){

            // Creating a block
            Mat block = planes[plane](Rect(j, i, 8, 8));

            // Converting the block to float
            block.convertTo( block, CV_64FC1 );

            // Applying dequantization
            if( plane == 0 ){
                multiply( block, mat1, block );
            }
            else {
                multiply( block, mat2, block );
            }

            // IDCT
            idct( block, block );

            // Adding 128 to the block
            add( block, 128.0, block );

            // Converting it back to unsigned int
            block.convertTo( block, CV_8UC1 );

            // Copying the block to the original image
            block.copyTo( planes[plane](Rect(j, i, 8, 8)) );
        }
    }
}

merge(planes, finalImage);
cvtColor( finalImage, finalImage, CV_YCrCb2BGR );

imshow("Decompressed image", finalImage);
waitKey(0);
imwrite(".../finalResult.jpg", finalImage);
}
