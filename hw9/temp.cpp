#include "cv.hpp"
#include <iostream> 

using namespace std; 
using namespace cv; 
int main()
{
 Mat src = imread("lena.png", 0); 
 if(src.empty())
 {
  cout << "the image is not exist" << endl; 
  return -1;
 }
 
 resize(src, src, Size(512, 512));
 src.convertTo(src, CV_32F, 8);
 Mat srcDCT; 
 dct(src, srcDCT);
 cout << srcDCT <<endl;


 imshow("src", src);
 imshow("dct", srcDCT);
 waitKey();
 return 0;
}