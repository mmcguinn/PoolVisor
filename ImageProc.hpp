#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string>
#include <map>
#include <boost/thread.hpp>

#include "ColorFilter.hpp"
#include "PoolBall.hpp"

#ifndef IMAGEPROC_HPP
#define IMAGEPROC_HPP

using namespace cv;
using namespace std;
using namespace boost;

class ImageProc
{
private:
  thread m_thread;
  VideoCapture m_video;
  ColorFilter m_feltFilter;
  map<string, ColorFilter> m_ballFilters;
  map<string, int> m_conf;
  
  vector<PoolBall> findBalls(Mat img, Mat oriMask);
public:
  ImageProc(string source);
  
  void start();
  void join();
  void process();
};

#endif