#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

struct MouseParams
{
    Mat img1, img2;
    vector<Point2f> in, out;
};

static void onMouse(int event, int x, int y, int, void *param)
{
    MouseParams *mp = (MouseParams *)param;
    Mat c_img = mp->img1;
    Mat t_img = mp->img2;
    Mat result;
    Mat sum;
    int count = 0;
    result = t_img.clone();

    // if (count < 4)
    // {
    if (event == EVENT_LBUTTONDOWN) // left button
    {

        // add(t_img,c_img,sum);
        mp->in.push_back(Point2f(x, y));
        // cout << mp->in[0].x << endl;
        // cout << mp->in[1].x << endl;
        // cout << mp->in[1].y << endl;
        // cout << mp->in[2].y << endl;
        // Size((mp->in[1].x - mp->in[0].x), (mp->in[3].y - mp->in[0].y))

        // result = t_img.clone();
    }
    if (event == EVENT_RBUTTONDOWN)
    {
        mp->in.clear();
        imshow("background", t_img);
    }
    // }

    //cout << count << endl;

    namedWindow("input");
    moveWindow("input", 800, 0);
    imshow("input", c_img);

    namedWindow("background");
    moveWindow("background", 0, 0);
    imshow("background", result);
    // waitKey(33);
}

// add활용해서 두 이미지 합치기
// polygon 사용

int main()
{
    VideoCapture contest("contest.mp4");
    VideoCapture timesq("Timesquare.mp4");

    MouseParams mp;
    Mat frame, image;
    Mat result;
    int flag = 0;
    double c_delay;
    int c_fps = contest.get(CAP_PROP_FPS); // frame rate
    c_delay = 1000 / c_fps;

    while (true)
    {

        contest >> image;
        timesq >> frame;

        if (image.empty())
        {
            cout << "end of video" << endl;
            break;
        }

        if (frame.empty())
        {
            cout << "end of video" << endl;
            break;
        }

        // cout << mp.out << endl;
    //if(flag ==0){
        mp.out.push_back(Point2f(0, 0));
        mp.out.push_back(Point2f(image.cols-1, 0));
        mp.out.push_back(Point2f(image.cols-1, image.rows-1));
        mp.out.push_back(Point2f(0, image.rows-1));
        flag = 1;
        
    
        mp.img1 = image;
        mp.img2 = frame;
        setMouseCallback("background", onMouse, (void *)&mp);

        if (mp.in.size() < 4)
        {
            for (size_t i = 0; i < mp.in.size(); i++)
            {
                circle(frame, mp.in[i], 3, Scalar(0, 0, 255), 3);
                // imshow("background", result);
            }
        }
        

        if (mp.in.size() == 4)
        {
            cout << mp.in[3].x << endl;
            cout << mp.in[3].y << endl;
            result = frame.clone();
            Mat homo_mat = getPerspectiveTransform(mp.out, mp.in);

            warpPerspective(image, result, homo_mat, Size(frame.cols, frame.rows)); //타임스퀘어에 나올 영상 사이즈
            imshow("temp", result);
            Point trapezoid[1][4];
            trapezoid[0][0] = Point(mp.in[0].x, mp.in[0].y);
            trapezoid[0][1] = Point(mp.in[1].x, mp.in[1].y);
            trapezoid[0][2] = Point(mp.in[2].x, mp.in[2].y);
            trapezoid[0][3] = Point(mp.in[3].x, mp.in[3].y);
            const Point *ppt[1] = {trapezoid[0]};
            int npt[] = {4};
            fillPoly(frame, ppt, npt, 1, Scalar(0, 0, 0), 8);
            frame += result;
            
        }

        namedWindow("input");
        moveWindow("input", 800, 0);
        imshow("input", image);

        namedWindow("background");
        moveWindow("background", 0, 0);
        imshow("background", frame);

        

        waitKey(33);
    }
}
