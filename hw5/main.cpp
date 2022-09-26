#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat drawLines(Mat left_src, Mat right_src, Mat result)
{
    vector<Vec2f> lines;
    float rho = 0, theta = 0, a, b, x0, y0;
    Point p1, p2;
    HoughLines(left_src, lines, 1, CV_PI / 180, 80, 0, 0, 30 * CV_PI / 180, 60 * CV_PI / 180);
    // HoughLines(left_src, lines, 1, CV_PI / 180, 150, 0, 0, 30 * CV_PI / 180, 60 * CV_PI / 180);
    for (int i = 0; i < lines.size(); i++)
    {
        rho += lines[i][0];
        theta += lines[i][1];
    }
    rho /= lines.size();
    theta /= lines.size();

    a = cos(theta);
    b = sin(theta);
    x0 = a * rho;
    y0 = b * rho;
    p1 = Point(cvRound(x0 + 1000 * (-b)) + 200, cvRound(y0 + 1000 * a) + 400);
    p2 = Point(cvRound(x0 - 1000 * (-b)) + 200, cvRound(y0 - 1000 * a) + 400);
    line(result, p1, p2, Scalar(0, 0, 255), 3, 0);

    HoughLines(right_src, lines, 1, CV_PI / 180, 80, 0, 0, 120 * CV_PI / 180, 150 * CV_PI / 180);
    rho = 0;
    theta = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        rho += lines[i][0];
        theta += lines[i][1];
    }
    rho /= lines.size();
    theta /= lines.size();

    a = cos(theta);
    b = sin(theta);
    x0 = a * rho;
    y0 = b * rho;
    p1 = Point(cvRound(x0 + 1000 * (-b)) + 600, cvRound(y0 + 1000 * a) + 400);
    p2 = Point(cvRound(x0 - 1000 * (-b)) + 600, cvRound(y0 - 1000 * a) + 400);
    line(result, p1, p2, Scalar(0, 0, 255), 3, 0);
    return result;
}

int main()
{
    Mat frame, canny_left, canny_right, gray, blurIm, canny;
    int fps;
    int delay;
    int total;

    Mat result;
    Point p1, p2;

    VideoCapture cap; // 카메라 또는 비디오 파일로 부터 프레임을 읽는데 필요한 정보

    if (cap.open("Road.mp4") == 0)
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }
    fps = cap.get(CAP_PROP_FPS); // 초당 프레임 수

    delay = 1000 / fps;
    while (1)
    {
        cap >> frame;
        if (cap.get(CAP_PROP_POS_MSEC) >= 20000)
        {
            break;
        }
        cvtColor(frame, gray, CV_BGR2GRAY);

        blur(gray, blurIm, Size(5, 5));
        Canny(blurIm, canny, 100, 200, 3);

        canny_left = canny(Range(400, 600), Range(200, 600));
        canny_right = canny(Range(400, 600), Range(600, 1000));
        // canny_right = canny;
        frame = drawLines(canny_left, canny_right, frame);

        imshow("video", frame);
        moveWindow("Left Canny", 200, 0);
        imshow("Left Canny", canny_left);
        moveWindow("Right Canny", 600, 0);
        imshow("Right Canny", canny_right);
        // imshow("trnas", result);
        waitKey(delay);
    }
}
