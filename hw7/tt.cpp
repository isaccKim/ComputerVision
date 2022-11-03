#include "cv.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

/*
Develop a program which finds the best matching image
among image DBs to the query image
•
Image DBs consists of 10 images
•
The user will enter the file name of the query image
•
If the file does not exist, the program should print out the error message such
as “No file!”
•
The image matching should be done by extracting features of
images and performing feature matching
•
The image sets will be given


*/

void draw(Mat query, int max_value)
{
    Mat image, descriptors1, descriptors2, max_image;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector<vector<DMatch> > matches; // DMatch is descriptor match
    vector<DMatch> goodMatches, max_goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int i, k;
    float nndr;
    string name;
    int max = 0;
    string temp;
    String path = "DBs/*.jpg";
    vector<String> str;

    glob(path, str, false);
    
    for (int j = 0; j <= str.size() - 1; j++)
    {
        image = imread(str[j]);

        if (query.empty() || image.empty())
        {
            cout << "No file!" << endl;
            return ;
        }

        resize(query, query, Size(640, 480));
        resize(image, image, Size(640, 480));

        // Compute ORB Features
        orbF->detectAndCompute(image, noArray(), keypoints1, descriptors1);
        orbF->detectAndCompute(query, noArray(), keypoints2, descriptors2);

        // KNN Matching(k-nearest neighbor matching)
        // Find best and second-best matches
        k = 2;
        matcher.knnMatch(descriptors1, descriptors2, matches, k);

        // Find out the best match is definitely better than the second-best match
        nndr = 0.5f;

        for (i = 0; i < matches.size(); i++)
        {
            if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance)
            {
                goodMatches.push_back(matches[i][0]);
                
            }
        }
        // Draws the found matches of keypointsfrom two images.
        
        if (goodMatches.size() >= max)
        {
            max = goodMatches.size();
        }
        if(max_value== goodMatches.size()){
                    drawMatches(image, keypoints1, query, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
                    cout << "hello" << endl;
                    break;
                }

        matches.clear();
        goodMatches.clear();
    }

    imshow("Query", query);
    imshow("imgMatches", imgMatches);

}

int main()
{
    Mat query, image, descriptors1, descriptors2, max_image;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2, max_keypoints1, max_keypoints2;
    vector<vector<DMatch> > matches; // DMatch is descriptor match
    vector<DMatch> goodMatches, max_goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int i, k;
    float nndr;
    string name;
    int max = 0;
    string best_image;
    string temp;
    String path = "DBs/*.jpg";
    vector<String> str;

    cout << "Enter query image name: " << endl;
    cin >> name;

    temp = "query_image/";
    temp += name;
    query = imread(temp);

    glob(path, str, false);

    cout << "[  INF0:0[ initialize OpenCL runtime..." << endl;
    
    for (int j = 0; j <= str.size() - 1; j++)
    {
        image = imread(str[j]);

        if (query.empty() || image.empty())
        {
            cout << "No file!" << endl;
            return -1;
        }

        resize(query, query, Size(640, 480));
        resize(image, image, Size(640, 480));

        // Compute ORB Features
        orbF->detectAndCompute(image, noArray(), keypoints1, descriptors1);
        orbF->detectAndCompute(query, noArray(), keypoints2, descriptors2);

        // KNN Matching(k-nearest neighbor matching)
        // Find best and second-best matches
        k = 2;
        matcher.knnMatch(descriptors1, descriptors2, matches, k);

        // Find out the best match is definitely better than the second-best match
        nndr = 0.5f;

        for (i = 0; i < matches.size(); i++)
        {
            if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance)
            {
                goodMatches.push_back(matches[i][0]);
            }
        }
        // Draws the found matches of keypointsfrom two images.
        if (goodMatches.size() >= max)
        {
            imgMatches.zeros(1,2, CV_8U);
            max = goodMatches.size();
            best_image = str[j];
        }
        cout << "Image number " << j + 1 << " Matching " << goodMatches.size() << endl;
        matches.clear();
        goodMatches.clear();
    }
    cout<< "hello" << endl;
    draw (query,max);
    waitKey(0);
}