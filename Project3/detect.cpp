#include "cv.hpp"
#include <iostream>
#include <opencv2/dnn.hpp>
#include <fstream>
using namespace cv;
using namespace std;
using namespace dnn;
int main(
    int argc,
    char **argv)
{
    String modelConfiguration = "deep /yolov2 -tiny.cfg";
    String modelBinary = "deep /yolov2 -tiny.weights";
    Netnet = readNetFromDarknet(modelConfiguration, modelBinary);
    VideoCapture cap("deep/downtown_road.mp4");
    vector<String> classNamesVec;
    ifstream classNamesFile("deep/coco.names");
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std ::getline(
            classNamesFile,
            className))
            classNamesVec.push_back(
                className);
    }
    while (1)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera /video or read image
        if (frame.empty())
        {
            waitKey();
            break;
        }
        if (
            frame.channels() == 4)
            cvtColor(
                frame,
                frame, COLOR_BGRA2BGR);
        // Convert Mat to batch of images
        Mat inputBlob =
            blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
        net.setInput(inputBlob, "data");                 // set the network input
        Mat detectionMat = net.forward("detection_out"); // compute output
        float confidenceThreshold = 0.24;                // by default
        for (int i = 0; i < detectionMat.rows; i++)
        {
            const int
                probability_index = 5;
            const int
                probability_size =
                    detectionMat.cols -
                    probability_index;
            float
                *
                    prob_array_ptr = &detectionMat.at<
                        float>(
                        i,
                        probability_index);
            size_t
                objectClass =
                    max_element(
                        prob_array_ptr,
                        prob_array_ptr +
                            probability_size) -
                    prob_array_ptr;
            //
            prediction probability of each class
                float
                    confidence =
                        detectionMat.at<
                            float>(
                            i, (
                                   int)
                                       objectClass +
                                   probability_index);
