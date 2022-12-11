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
      imshow("ll",left_src);
    imshow("rr",right_src);
    vector<Vec2f> lines;
    float rho = 0, theta = 0, a, b, x0, y0;
    Point p1, p2;

    HoughLines(left_src, lines, 1, CV_PI / 180, 60, 0, 0, CV_PI / 180 * 90, CV_PI / 180 * (180));
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
    p1 = Point(cvRound(x0 + 1000 * (-b)) + 300, cvRound(y0 + 1000 * a) + 330);
    p2 = Point(cvRound(x0 - 1000 * (-b)) + 300, cvRound(y0 - 1000 * a) + 330);

    // ROI를 하며 바뀐 중심측
    if (p1 != Point(300, 330) && p2 != Point(300, 330))
    {
        // line(result, p1, p2, Scalar(0, 0, 255), 4, 0); // x,y,좌표가 없을 경우 350,400좌표에 점이 생기는 것을 막기위한 if문
        return 1;
    }

    HoughLines(right_src, lines, 1, CV_PI / 180, 62, 0, 0, CV_PI / 180 * 140, CV_PI / 180 * 180);

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
    if (p1 != Point(350, 330) && p2 != Point(350, 330))
    {
        // line(result, p1, p2, Scalar(0, 0, 255), 4, 0);
        return 2;
    }
    return 0;
}

int main(int argc, char **argv)
{
    Mat frame, canny_left, canny_right, gray, blurIm, canny; // LineDetection

    Mat foregroundMask, backgroundImg, foregroundImg;
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2(); // BackGraoundSubstraction

    String modelConfiguration = "deep/yolov2-tiny.cfg";
    String modelBinary = "deep/yolov2-tiny.weights";
    Net net = readNetFromDarknet(modelConfiguration, modelBinary);
    vector<String> classNamesVec;
    ifstream classNamesFile("deep/coco.names");

    // VideoCapture cap("deep/Project3_1.mp4");
    VideoCapture cap("deep/Project3_1.mp4");
    int fps;
    int delay;
    int total;
    fps = cap.get(CAP_PROP_FPS);
    total = cap.get(CAP_PROP_FRAME_COUNT);
    delay = 1000 / fps;

    int min = 9999;
    int frontCarMoving = 0;
    float confidenceThreshold = 0.24;

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
        int count = 0;
        int carMoving = 0;

        Mat front_img = frame(Rect(245, 230, 195, 220));
        Mat sky_img = frame(Rect(400, 50, 150, 150));

        if (foregroundMask.empty())
            foregroundMask.create(sky_img.size(), sky_img.type());
        bg_model->apply(sky_img, foregroundMask);
        GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);
        threshold(foregroundMask, foregroundMask, 150, 255, THRESH_BINARY);
        foregroundImg = Scalar::all(0);
        sky_img.copyTo(foregroundImg, foregroundMask);
        for (int i = 0; i < foregroundMask.rows; i++)
        {
            for (int j = 0; j < foregroundMask.cols; j++)
            {
                if (foregroundMask.at<uchar>(i, j) > 0)
                {
                    count++;
                }
            }
        }
        // Moving Detection
        if (count > 1000)
        {
            carMoving = 1;
            frontCarMoving = 0;
        }

        // front 이미지 시작
        if (front_img.channels() == 4)
            cvtColor(front_img, front_img, COLOR_BGRA2BGR); // ConvertMattobatchof images
        Mat inputBlob_1 = blobFromImage(front_img, 1 / 255.F, Size(416, 416), Scalar(), true, false);
        net.setInput(inputBlob_1, "data");                 // setthenetworkinput
        Mat detectionMat_1 = net.forward("detection_out"); // computeoutput

        // bydefault
        for (int i = 0; i < detectionMat_1.rows; i++)
        {
            const int probability_index = 5;
            const int probability_size = detectionMat_1.cols - probability_index;
            float *prob_array_ptr = &detectionMat_1.at<float>(i, probability_index);
            size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr; // prediction probability of each class
            float confidence = detectionMat_1.at<float>(i, (int)objectClass + probability_index);
            // for drawing labels with name and confidence

            if (confidence > confidenceThreshold)
            {
                float x_center = detectionMat_1.at<float>(i, 0) * front_img.cols;
                float y_center = detectionMat_1.at<float>(i, 1) * front_img.rows;
                float width = detectionMat_1.at<float>(i, 2) * front_img.cols;
                float height = detectionMat_1.at<float>(i, 3) * front_img.rows;
                Point p1(cvRound(x_center - width / 2), cvRound(y_center - height / 2));
                Point p2(cvRound(x_center + width / 2), cvRound(y_center + height / 2));
                Rect object(p1, p2);
                Scalar object_roi_color(0, 0, 255);

                String className = objectClass < classNamesVec.size() ? classNamesVec[objectClass] : cv::format("unknown(%d)", objectClass);
                String label = format("%s: %.2f", className.c_str(), confidence);
                int baseLine = 0;
                Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                String c = "car";

                if (className.c_str() == c && p2.y - p1.y > 70)
                {
                    if (p2.y - p1.y < min)
                    {
                        min = p2.y - p1.y;
                        frontCarMoving = 1;
                        min = 99999; // 자동차가 움직이면 값 초기화 
                    }
                }
            }
        }

        if (carMoving == 0 & frontCarMoving == 1)
        {
            putText(frame, "Start Moving!", Point(front_img.cols/2-20, front_img.rows / 4), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(0, 255, 255), 2);
        }
        
        // YOLO
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

                if (className.c_str() == p && p2.x - p1.x > 55)
                {
                    putText(frame, "Human detected nearby!", Point(frame.cols / 3 - 20, frame.rows / 3), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
                }

                else if (className.c_str() == c && p2.x - p1.x > 150)
                {
                    putText(frame, "Car detected nearby!", Point(frame.cols / 4-50, frame.rows / 2-50), FONT_HERSHEY_SIMPLEX, 1.3, Scalar(0, 0, 255), 2);
                }

            }
        }
        cvtColor(frame, gray, CV_BGR2GRAY);

        blur(gray, blurIm, Size(5, 5));
        Canny(blurIm, canny, 10, 60, 3);

        canny_left = canny(Range(330, 480), Range(300, 350));
        canny_right = canny(Range(330, 480), Range(350, 400));

        if (drawLines(canny_left, canny_right, frame) != 0)
        {
            putText(frame, "Lane Departure!", Point(frame.cols / 3 - 20, frame.rows / 3), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
        }

        
        imshow("Project3", frame);
        if (frame.empty())break;

        waitKey(delay);
    }
    return 0;
}
