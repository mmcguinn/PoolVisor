#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string>
#include <map>
#include <boost/thread.hpp>

#include "ColorFilter.hpp"
#include "PoolBall.hpp"
#include "TableState.hpp"

#ifndef IMAGEPROC_HPP
#define IMAGEPROC_HPP

using namespace std;
using namespace boost;

class ImageProc
{
private:
  thread m_thread, m_readThread;
  mutex m_readMutex;
  cv::Mat m_nextFrame;
  cv::VideoCapture m_video;
  ColorFilter m_feltFilter;
  map<string, ColorFilter> m_ballFilters;
  map<string, int> m_conf;
  TableState& m_state;
  bool m_running;
  
  vector<PoolBall> findBalls(cv::Mat img, cv::Mat oriMask);
  void frameSkipper();
public:
  ImageProc(string source, TableState& state);
  
  void start();
  void join();
  void process();
};

#endif