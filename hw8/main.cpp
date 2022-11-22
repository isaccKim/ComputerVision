#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

// Assignment 8
// •Develop a program which detects a face at certain distance
// •Read Faces.mp4
// •When the user presses ‘n’, it should detect the nearest face
// •When the user presses ‘f’, it should detect the farthest face
// •When the user presses ‘m’, it should detect the face at the middle range
// •When the user presses ‘r’, it should reset program
// •The program should display text where the face is(n,m,f ) in the bounding rectangle
// •When the user presses ‘t’, it should track only the region of face you chose
// •If the user presses ‘n’(nearest), and next press ‘t’(tracking), your program should display another window to show
// face-> ‘tracking’. Another window is just background(255,0,0)->(blue)
// •If the user presses ‘t’ without previously pressing any keys(n,f,m), the program should alert
// text message(“Detect before tracking”) to user. It is impossible to track without any detection.
// • When the user presses ‘t’ again, program should destroy ‘tracking’ window
// •f :  min[35] , max[45]
// •m : min[45] , max[60]
// •n :  min[65] , max[80]

int main()
{
    CascadeClassifier face_classifier;
    Mat frame, grayframe;

    Mat result, bgdModel, fgdModel, foreground;

    vector<Rect> faces;
    int i, fps, delay;
    int temp;
    int type, detect_on = 0;
    int flag = 0, flag_2 = 0, flag_3 = 0;
    // openthe webcam
    VideoCapture cap;

    // check if we succeeded
    if (cap.open("Faces.mp4") == 0)
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }

    fps = cap.get(CAP_PROP_FPS); // 초당 프레임 수

    delay = 1000 / fps;

    face_classifier.load("haarcascade_frontalface_alt.xml");

    while (true)
    {
        cap >> frame;

        temp = waitKey(delay);

        if (temp == 102 || temp == 109 || temp == 110)
        {
            type = temp;
            flag = 1;
            flag_2 = 0;
        }
        if (temp == 114)
        {
            type = 114;
            flag_2 = 0;
        }

        if (detect_on == 116 && temp == 116)
        {
            detect_on = 0;
            destroyWindow("tracking");
            type = 0;
            flag = 0;
            temp = -1;
        }
        
        if (temp == 116 && flag == 1)
        {
            detect_on = temp;
        }

        if (flag_2 == 1)
        {
            putText(frame, "Detect before tracking", Point(20, 60), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 1);
        }
        else if (temp == 116 && flag == 0)
        {
            flag_2 = 1;
        }

        cvtColor(frame, grayframe, COLOR_BGR2GRAY);

        // face_classifier.detectMultiScale(grayframe, fa2ces, 1.1, 3, 0, Size(30, 30));
        // draw the results

        // print the output

        if (type == 102)
        {
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(25, 25), Size(40, 40));
        } // f : farthest

        else if (type == 109)
        {
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(55, 55), Size(60, 60));
        } // m : middle range

        else if (type == 110)
        {
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(76, 76), Size(85, 85));
        } // n : nearest

        else if (type == 114)
        {
            cap >> frame;
            flag = 0;
            flag_2 = 0;
            type = 0;
            detect_on = 0;
            destroyWindow("tracking");
        } // r: reset

        for (i = 0; i < faces.size(); i++)
        {
            if (detect_on == 116 && flag == 1)
            {
                Rect rectangle(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
                grabCut(frame, result, rectangle, bgdModel, fgdModel, 4, GC_INIT_WITH_RECT);
                compare(result, GC_PR_FGD, result, CMP_EQ);
                foreground = Mat(frame.size(), CV_8UC3, Scalar(255, 0, 0));
                frame.copyTo(foreground, result);
                imshow("tracking", foreground);
            }
            if (type == 102 || type == 109 || type == 110)
            {
                char key;
                Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
                Point tr(faces[i].x, faces[i].y);
                if(type == 102){
                    putText(frame,"f", Point(faces[i].x+20, faces[i].y+20), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255,0), 1);
                }
                if(type == 109){
                    putText(frame,"m", Point(faces[i].x+20, faces[i].y+20), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255,0), 1);
                }
                if(type == 110){
                    putText(frame,"n", Point(faces[i].x+20, faces[i].y+20), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255,0), 1);
                }
                
                rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
            }
        }
        // ESC
        if (type == 27)
            break;

        if(!frame.empty()){
            imshow("Faces", frame);
        }
        else {
            break;
        }

        waitKey(delay);
    }
}
