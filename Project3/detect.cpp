#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int drawLines(Mat left_src, Mat right_src, Mat result)
{
    vector<Vec2f> lines;
    float rho = 0, theta = 0, a, b, x0, y0;
    Point p1, p2; 

    HoughLines(left_src, lines, 1, CV_PI / 180, 60, 0, 0,  CV_PI / 180*20, CV_PI / 180 * (180)); 
    // HoughLines : 일반 평면에 있던 점들을 다른 좌표평면에 직선으로 나타내고 그값을 lines애 저장
    for (int i = 0; i < lines.size(); i++)
    {
        rho += lines[i][0];
        theta += lines[i][1];
    }
    rho /= lines.size();  
    theta /= lines.size();

    a = cos(theta);
    b = sin(theta);
    x0 = a * rho; // 평면 옮기는 작업
    y0 = b * rho;
    p1 = Point(cvRound(x0 + 1000 * (-b)) + 290, cvRound(y0 + 1000 * a) + 330);
    p2 = Point(cvRound(x0 - 1000 * (-b)) + 290, cvRound(y0 - 1000 * a) + 330);


    // ROI를 하며 바뀐 중심측
    if(p1 != Point(290,330) && p2 != Point(290,330)){
        line(result, p1, p2, Scalar(0, 0, 255), 4, 0); // x,y,좌표가 없을 경우 350,400좌표에 점이 생기는 것을 막기위한 if문
        return 1;
    } 

    HoughLines(right_src, lines, 1, CV_PI / 180, 62, 0, 0,  CV_PI / 180*140, CV_PI / 180*180 );
    
    
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
    p1 = Point(cvRound(x0 + 1000 * (-b)) + 350, cvRound(y0 + 1000 * a) + 330);
    p2 = Point(cvRound(x0 - 1000 * (-b)) + 350, cvRound(y0 - 1000 * a) + 330);
    if(p1 != Point(350,330) && p2 != Point(350,330)){
        line(result, p1, p2, Scalar(0, 0, 255), 4, 0);
        return 2;
    }
    return 0;
}

int main()
{
    Mat frame, canny_left, canny_right, gray, blurIm, canny;
    int fps;
    int delay;


    VideoCapture cap; // 카메라 또는 비디오 파일로 부터 프레임을 읽는데 필요한 정보
    


    if (cap.open("deep/Project3_1.mp4") == 0)
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }
    fps = cap.get(CAP_PROP_FPS); // 초당 프레임 수

    delay = 1000 / fps;
    while (1)
    {
        cap >> frame;
     
        cvtColor(frame, gray, CV_BGR2GRAY);

        blur(gray, blurIm, Size(5, 5)); 
        Canny(blurIm, canny, 10, 60, 3);
                //  x   y     x   y
                // 290,341 / 350 480 
                // 350,341 / 410 480
                
        canny_left = canny(Range(330, 480), Range(290, 350));
        canny_right = canny(Range(330, 480), Range(350, 410)); 
      
        // if(drawLines(canny_left, canny_right, frame) == 2){
        //     putText(frame, " Rihgt Line Detection", Point(frame.cols/3,frame.rows/3), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0));

        // }

          if(drawLines(canny_left, canny_right, frame) != 0){
            putText(frame, "Lane Departure", Point(frame.cols/3-20,frame.rows/3), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,0,0),2);
        }
        // cout << frame.cols << endl;
        // cout << frame.rows << endl;
        rectangle(frame, Rect(Point(240,330),Point(440,360)), Scalar(0, 0, 255), 1, 8, 0); // ROI

        rectangle(frame, Rect(Point(350,330),Point(410,480)), Scalar(0, 0, 255), 1, 8, 0);
        rectangle(frame, Rect(Point(290,330),Point(350,480)), Scalar(0, 0, 255), 1, 8, 0);
        
        imshow("video", frame);
        moveWindow("Left Canny", 350, 0);
        imshow("Left Canny", canny_left);
        moveWindow("Right Canny", 480, 0);
        imshow("Right Canny", canny_right);
        
        // waitKey(delay);
        waitKey(1);
    }
}
