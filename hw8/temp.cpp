#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
struct CallbackParam
{
    Mat frame;
    Point pt1, pt2;
    Rect roi;
    bool drag;
    bool updated;
};
// if click and drag mouse on the window
void onMouse(int event, int x, int y, int flags, void *param)
{
    CallbackParam *p = (CallbackParam *)param; // clicked
    if (event == EVENT_LBUTTONDOWN)
    {
        p->pt1.x = x;
        p->pt1.y = y;
        p->pt2 = p->pt1;
        p->drag = true;
    }
    // unclicked
    if (event == EVENT_LBUTTONUP)
    {
        int w = x - p->pt1.x;
        int h = y - p->pt1.y;
        p->roi.x = p->pt1.x;
        p->roi.y = p->pt1.y;
        p->roi.width = w;
        p->roi.height = h;
        p->drag = false;
        if (w >= 10 && h >= 10)
        {
            p->updated = true;
        }
    }
    // clicked and moving
    if (p->drag && event == EVENT_MOUSEMOVE)
    {
        if (p->pt2.x != x || p->pt2.y != y)
        {
            Mat img = p->frame.clone(); // copy paused image 
            p->pt2.x = x; // update
            p->pt2.y = y; // dst point
            rectangle(img, p->pt1, p->pt2, Scalar(0, 255, 0), 1);
            imshow("Tracker", img);
        }
    }
}

int main(int argc, char *argv[])
{ // open the webcam 
    VideoCapture cap(0);
    CallbackParam param;
    Mat frame, m_backproj, hsv;
    Mat m_model3d;
    Rect m_rc;
    // for HSV colorspace
    float hrange[] = {0, 180};                        // Hue
    float srange[] = {0, 255};                        // Saturation 
    float vrange[] = { 0,255 }; // Brightness
    const float *ranges[] = {hrange, srange, vrange}; // hue, saturation, brightness 
    int channels[] = { 0, 1, 2 };
    int hist_sizes[] = {16, 16, 16};
    // check if we succeeded 
    if (!cap.isOpened()){
    cout << "can't open video file" << endl;
    return 0;
}
// click and drag on image to set ROI 
    cap >> frame;
    imshow("Tracker", frame);
    param.frame = frame;
    param.drag = false;
    param.updated = false;
// if mouse event is occured, it calls onMouse function 
setMouseCallback("Tracker", onMouse, &param);
bool tracking = false;
while (true)
{
    // image acquisition & target init 
    if (param.drag){
    if (waitKey(33) == 27)break; // ESC key
    continue;
}
// convert image from RGB to HSV 
cvtColor(frame, hsv, COLOR_BGR2HSV); 
if (param.updated){
Rect rc = param.roi;
Mat mask = Mat::zeros(rc.height, rc.width, CV_8U);
ellipse(mask, Point(rc.width / 2, rc.height / 2), Size(rc.width / 2, rc.height / 2), 0, 0, 360, 255, CV_FILLED);
Mat roi(hsv, rc);
// histogram calculation
calcHist(&roi,     // The source array(s)
        1,        // The number of source arrays
        channels, // The channel (dim) to be measured.
        mask,     // A mask to be used on the source array (zeros indicating pixels to be ignored) 
        m_model3d, // The Mat object where the histogram will be stored
        3,
        hist_sizes,
        ranges
        );
        m_rc = rc;
        param.updated = false;
        tracking = true;
        }
// get a new frame from webcam
    cap >> frame;
if (frame.empty())break; // if there are no frames to read, quit.
// image processing
 if (tracking){
// histogram backprojection.
// all the arguments are known (the same as used to calculate the histogram), // only we add the backproj matrix,
// which will store the backprojection of the source image (&hue) 
calcBackProject(&hsv, 1, channels, m_model3d, m_backproj, ranges);
// tracking[meanShift]
// obtain a window with maximum pixel distribution 
meanShift(m_backproj, // dst
m_rc,                                                            // initial location of window
    TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1) // termination criteria
     );
    rectangle(frame, m_rc, Scalar(0, 0, 255), 3);
}
// image display 
imshow("Tracker", frame);
// user input
char ch = waitKey(33);
if (ch == 27)
    break; // ESC Key (exit)
else if (ch == 32)
{ // SPACE Key (pause video)
    // exit from while loop if user input is SPACE or ESC key while ((ch = waitKey(33)) != 32 && ch != 27);
    // if user input is not SPACE but ESC, quit program.
    if (ch == 27)
        break;
}
}
return 0;
}

CamShift(m_backproj, // dst 
m_rc, // initial location of window 
cvTermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 20,1) // termination criteria 
); 
rectangle(frame, m_rc, Scalar(0, 0, 255), 3);
}
// image display 
imshow("Tracker", frame);
// user input
char ch = waitKey(33);
if (ch == 27)
    break; // ESC Key (exit)
else if (ch == 32)
{ // SPACE Key (pause video)
    // exit from while loop if user input is SPACE or ESC key while ((ch = waitKey(33)) != 32 && ch != 27);
    // if user input is not SPACE but ESC, quit program.
    if (ch == 27)
        break;
}
}
return 0;
}


