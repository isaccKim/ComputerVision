#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat frame, canny_left, canny_right,gray, blur, canny;
    int fps;
    int delay;
    int total;
    Mat result;
    Point p1, p2;
    vector<Vec2f> lines;
    float rho, theta, a, b, x0, y0;
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
     if (cap.get(CAP_PROP_POS_MSEC) >=20000)
        {
            break;
        }

        cvtColor(frame, gray, CV_BGR2GRAY );
        
        GaussianBlur(gray,blur,Size(5,5),5,5,BORDER_DEFAULT);
        
        Canny(blur,canny,10,60,3);

        canny_left = canny(Range(400, 600), Range(200, 600));
        canny_right = canny(Range(400, 600), Range(600, 1000));

        // HoughLines(canny, lines, 1, CV_PI / 180, 150,0,CV_PI);
        
        // for (int i = 0; i < lines.size(); i++)
        // {
        //     rho = lines[i][0];
        //     theta = lines[i][1];
        //     a = cos(theta);
        //     b = sin(theta);
        //     x0 = a * rho;
        //     y0 = b * rho;
        //     p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
        //     p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));
        //     line(result, p1, p2, Scalar(0, 0, 255), 3, 8);
        // }

        imshow("video", frame);
        moveWindow("Left Canny", 200, 0); 
        imshow("Left Canny", canny_left);
        moveWindow("Right Canny", 600, 0);
        imshow("Right Canny", canny_right);
        imshow("trnas",result);
        waitKey(delay);
    }
}

