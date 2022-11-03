#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main()
{
    Mat result, bgdModel, fgdModel, image, foreground;
    image = imread("dog.png");
    // inner rectangle which includes foreground
    Rect rectangle(15, 0, 155, 240);
    grabCut(image, result, rectangle, bgdModel, fgdModel, 10, GC_INIT_WITH_RECT);
    compare(result, GC_PR_FGD, result, CMP_EQ);
    foreground = Mat(image.size(), CV_8UC3, Scalar(255, 255, 255));
    image.copyTo(foreground, result);
    imshow("original", image);
    imshow("Result", result);
    imshow("Foreground", foreground);
    waitKey(0);
    return 0;
}
