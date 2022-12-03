/*
Develop a program which does the following tasks.
1) Alert a warning text “Lane departure!” when lane departure of our 
car is detected.
2) Alert a warning text “Start Moving!” when our car is stationary and 
the car in front starts moving from a stop.
3-1) Alert a warning text “Human detected nearby!” when person or 
people are detected nearby while driving.
3-2) Alert a warning text “Car detected nearby!” when car or cars are 
detected nearby while driving.

Read the given video.
•For task 1, you may use HoughLine
•For task 2~3-2, you should use YOLO to solve them.
• The color and the position of the warning texts does not matter. Just be sure it can be visible.
• Your program should display one window ‘Project3


차 인식
앞차가 움직이면 움직여라고 말하기 
차선 이탈 감지

알림 창이 같이 나타날 수도 있다. 

result 2
사람 인식

차선이 없으면 인식 안합니다. 
*/


#include "cv.hpp"
#include <iostream>
#include <opencv2/dnn.hpp>
#include <fstream>

using namespace cv;
using namespace std;
using namespace dnn;

int drawLines(Mat left_src, Mat right_src, Mat result)
{
    vector<Vec2f> lines;
    float rho = 0, theta = 0, a, b, x0, y0;
    Point p1, p2; 

    HoughLines(left_src, lines, 1, CV_PI / 180, 50, 0, 0,  CV_PI / 180 * 70, CV_PI / 180 * 80); 
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

    cout << p1 << endl;
    cout << p2 << endl;
    // ROI를 하며 바뀐 중심측
    if(p1 != Point(290,330) && p2 != Point(290,330)){
        line(result, p1, p2, Scalar(0, 0, 255), 4, 0); // x,y,좌표가 없을 경우 350,400좌표에 점이 생기는 것을 막기위한 if문
        return 1;
    } 

    HoughLines(right_src, lines, 1, CV_PI / 180, 62, 0, 0,  CV_PI / 180, CV_PI / 180*180 );
    cout <<  CV_PI / 180<< endl;
    cout <<  CV_PI / 180<< endl;
    
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
        return 1;
    }
    return 0;
}


int main(int argc, char **argv)
{
    Mat frame, canny_left, canny_right, gray, blurIm, canny;
    String modelConfiguration = "deep/yolov2-tiny.cfg";
    String modelBinary = "deep/yolov2-tiny.weights";
    Net net = readNetFromDarknet(modelConfiguration, modelBinary);
    VideoCapture cap("deep/Project3_1.mp4");
    vector<String> classNamesVec;
    ifstream classNamesFile("deep/coco.names");
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className))
            classNamesVec.push_back(className);
    }
    while (1)
    {
        
        cap >> frame; // getanewframefromcamera/videoorreadimage
        if (frame.empty())
        {
            waitKey();
            break;
        }
        if (frame.channels() == 4)
            cvtColor(frame, frame, COLOR_BGRA2BGR); // ConvertMattobatchof images
        Mat inputBlob = blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
        net.setInput(inputBlob, "data");                 // setthenetworkinput
        Mat detectionMat = net.forward("detection_out"); // computeoutput
        float confidenceThreshold = 0.24;                // bydefault
        for (int i = 0; i < detectionMat.rows; i++)
        {
            const int probability_index = 5;
            const int probability_size = detectionMat.cols - probability_index;
            float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);
            size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr; // prediction probability of each class
            float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
            // for drawing labels with name and confidence
            if (confidence > confidenceThreshold)
            {
                float x_center = detectionMat.at<float>(i, 0) * frame.cols;
                float y_center = detectionMat.at<float>(i, 1) * frame.rows;
                float width = detectionMat.at<float>(i, 2) * frame.cols;
                float height = detectionMat.at<float>(i, 3) * frame.rows;
                Point p1(cvRound(x_center - width / 2), cvRound(y_center - height / 2));
                Point p2(cvRound(x_center + width / 2), cvRound(y_center + height / 2));
                Rect object(p1, p2);
                Scalar object_roi_color(0, 255, 0);
                rectangle(frame, object, object_roi_color);
                String className = objectClass < classNamesVec.size() ? classNamesVec[objectClass] : cv::format("unknown(%d)", objectClass);
                String label = format("%s: %.2f", className.c_str(), confidence);
                int baseLine = 0;
                Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                String p = "person";
                String c = "car";

                cout << frame.rows << endl;
                cout << frame.cols << endl;
                if(className.c_str() == p && p2.x-p1.x>50){
                // cout << (p2.x-p1.x) <<endl;
                putText(frame, "Human detected nearby!", Point(frame.cols/3,frame.rows/3), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0));
                }

                else if(className.c_str() == c && p2.x-p1.x>100){
                // cout << (p2.x-p1.x) <<endl;
                putText(frame, "Car detected nearby!", Point(frame.cols/3,frame.rows/2), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0));
                }
                
                // rectangle(frame, Rect(Point(350,410),Point(460,470)), Scalar(0, 0, 255), 1, 8, 0);
                // rectangle(frame, Rect(Point(240,410),Point(350,470)), Scalar(0, 0, 255), 1, 8, 0);

        
                
                cout << frame.cols/2 << endl;
            }
        }
        imshow("YOLO: Detections", frame);
        if (waitKey(1) >= 0)
            break;
    }
    return 0;
}
