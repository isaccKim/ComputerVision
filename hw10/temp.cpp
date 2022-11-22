
#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

struct MouseParams
{
    Mat img;
    vector<Point2f> in, out;
};
static void onMouse(int event, int x, int y, int, void *param)
{
    MouseParams *mp = (MouseParams *)param;
    Mat img = mp->img;
    if (event == EVENT_LBUTTONDOWN) // left button
    {
        mp->out.push_back(Point2f(x, y));
        cout << x << " " << y << endl;

    } // Resetpositions
}
int main()
{
    Mat frame1, frame2;
    int fps;
    int delay;
    int total;
    int flag = 0;
    int flag_2 = 0;
    VideoCapture cap_1;
    VideoCapture cap_2;
    Mat imput, f_result;
    MouseParams mp;
    Mat result, bgdModel, fgdModel, foreground;

    if (cap_1.open("Timesquare.mp4") == 0)
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }

    if (cap_2.open("contest.mp4") == 0)
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }

    fps = cap_1.get(CAP_PROP_FPS);

    delay = 1000 / fps;

    while (1)
    {
        cap_1 >> frame1;
        cap_2 >> frame2; // input
        imput = frame2;
        mp.img = imput;
        if (flag_2 == 0)
        {
            mp.in.push_back(Point2f(0, 0));
            mp.in.push_back(Point2f(frame2.cols - 1, 0));
            mp.in.push_back(Point2f(frame2.cols - 1, frame2.rows - 1));
            mp.in.push_back(Point2f(0, frame2.rows - 1));
            flag_2 = 1;

            for (size_t i = 0; i < mp.in.size(); i++)
            {
                cout << mp.in[i] << endl;
            }
        }
        if(mp.out.size()<4){
            for (size_t i = 0; i < mp.out.size(); i++)
        {
            circle(frame1, mp.out[i], 3, Scalar(0, 0, 255), 5);
        }
        }
        

        if (mp.out.size() == 4)
        {
            mp.img = frame2;
            Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);
            warpPerspective(mp.img, result, homo_mat, Size(frame1.cols, frame1.rows));

            Point trapezoid[1][4];
            trapezoid[0][0] = Point(mp.out[0].x,mp.out[0].y);
            trapezoid[0][1] = Point(mp.out[1].x,mp.out[1].y);
            trapezoid[0][2] = Point(mp.out[2].x,mp.out[2].y);
            trapezoid[0][3] = Point(mp.out[3].x,mp.out[3].y);

          const Point* ppt[1] = { trapezoid[0] };
          int npt[] = { 4 };
          fillPoly(frame1, ppt, npt, 1, Scalar(0, 0, 0), 8);


            frame1 += result;
        }

        imshow("background", frame1);
        imshow("input", frame2); // input

        setMouseCallback("background", onMouse, (void *)&mp);

        waitKey(delay);
    }

    return 0;
}
