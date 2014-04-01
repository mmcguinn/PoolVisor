#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "ColorFilter.hpp"
#include "PoolBall.hpp"
#include "ImageProc.hpp"

using namespace cv;
using namespace std;

//g++ -o PoolVisor test.cpp  `pkg-config --cflags --libs opencv`

int h1,s1,v1,h2,s2,v2;

void updateTest(int, void*)
{
  Mat img;
  if(!(img=imread("pool2.jpg", 1)).data)
  {
      return;
  }
  cvtColor(img,img, CV_BGR2HSV);
  ColorFilter filter(Scalar(h1,s1,v1), Scalar(h2,s2,v2));
  Mat mask = filter.makeMask(img, false);
  cvtColor(mask, mask, CV_GRAY2BGR);
  bitwise_and(img, mask, img);
  cvtColor(img,img, CV_HSV2BGR);
  imshow("panel", img);
}

void startTest()
{
  Mat img;
  if(!(img=imread("pool2.jpg", 1)).data)
  {
      return;
  }
  
  imshow("panel", img);
  
  namedWindow("slider", 1);
  createTrackbar("h1", "slider", &h1, 180, updateTest);
  createTrackbar("s1", "slider", &s1, 255, updateTest);
  createTrackbar("v1", "slider", &v1, 255, updateTest);
  createTrackbar("h2", "slider", &h2, 180, updateTest);
  createTrackbar("s2", "slider", &s2, 255, updateTest);
  createTrackbar("v2", "slider", &v2, 255, updateTest);
  waitKey(0);
}

int main(int argc, char* argv[])
{
    ImageProc proc("pool%01d.jpg");
    //proc.start();
    //proc.join();
    proc.process();
    
    startTest();
    return 0;
}
