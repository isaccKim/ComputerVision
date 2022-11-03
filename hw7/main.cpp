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

int draw(string query_path,string str,int mode)
{
    Mat query, image, descriptors1, descriptors2, max_image;
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
    

    query = imread(query_path);
    image = imread(str);

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
        k = 3;
        matcher.knnMatch(descriptors1, descriptors2, matches, k);

        // Find out the best match is definitely better than the second-best match
        nndr = 0.6039f;

        for (i = 0; i < matches.size(); i++)
        {
            if (matches.at(i).size() == 3 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance)
            {
                goodMatches.push_back(matches[i][0]);
                
            }
        }
        // Draws the found matches of keypointsfrom two images.
        
        if (goodMatches.size() >= max)
        {
            max = goodMatches.size();
        }

        if(mode ==1){
            drawMatches(image, keypoints1, query, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
            imshow("Query", query);
            imshow("Best_matching", imgMatches);
            return 0;
        }


        return goodMatches.size();
        }

int main()
{
    Mat query;
    string name;
    int max = 0;
    string query_path;
    String path = "DBs/*.jpg";
    vector<String> str;
    string max_path ;
    cout << "Enter query image name: " << endl;
    cin >> name;
    query_path = "query_image/";
    query_path += name;

    glob(path, str, false);    

    
    for(int i=0; i<str.size(); i++){
        int temp = draw(query_path,str[i],0);
        if(temp == -1)return 0;
        cout << "Image number " << i + 1 << " Matching " << temp << endl;
        if(temp>max){
            max_path =str[i];
            max = temp;
        }
    }

    draw (query_path,max_path,1); // darw max

    waitKey(0);
    return 0;
}