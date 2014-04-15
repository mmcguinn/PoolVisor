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
#include "LogicProc.hpp"
#include "StreamProc.hpp"
#include "TableState.hpp"


using namespace std;

//g++ -o PoolVisor test.cpp  `pkg-config --cflags --libs opencv`

int h1,s1,v1,h2,s2,v2;

void updateTest(int, void*)
{
  cv::Mat img;
  if(!(img=cv::imread("test.jpg", 1)).data)
  {
      return;
  }
  cv::cvtColor(img,img, CV_BGR2HSV);
  ColorFilter filter(cv::Scalar(h1,s1,v1), cv::Scalar(h2,s2,v2));
  cv::Mat mask = filter.makeMask(img, false);
  cv::cvtColor(mask, mask, CV_GRAY2BGR);
  cv::bitwise_and(img, mask, img);
  cv::cvtColor(img,img, CV_HSV2BGR);
  cv::imshow("panel", img);
}

void startTest()
{
  cv::Mat img;
  if(!(img=cv::imread("test.jpg", 1)).data)
  {
      return;
  }
  
  cv::imshow("panel", img);
  
  cv::namedWindow("slider", 1);
  cv::createTrackbar("h1", "slider", &h1, 180, updateTest);
  cv::createTrackbar("s1", "slider", &s1, 255, updateTest);
  cv::createTrackbar("v1", "slider", &v1, 255, updateTest);
  cv::createTrackbar("h2", "slider", &h2, 180, updateTest);
  cv::createTrackbar("s2", "slider", &s2, 255, updateTest);
  cv::createTrackbar("v2", "slider", &v2, 255, updateTest);
  
  while (1)
  {
    cv::waitKey(0);
  }
}

int main(int argc, char* argv[])
{
    TableState state(30);
    
    ImageProc iproc("/dev/stdin", state);
    iproc.process();
    
//     LogicProc lproc("/dev/video0", state);
//     lproc.start();
    
//     lproc.join();
//     iproc.join();
//     
//     StreamProc streamer("/dev/video0", 640, 480);
//     streamer.process();
    
    startTest();
    return 0;
}
