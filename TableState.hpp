#include <cv.h>
#include <math.h>
#include <string>
#include <map>
#include <deque>
#include <boost/thread.hpp>

#include "PoolBall.hpp"

#ifndef TABLESTATE_HPP
#define TABLESTATE_HPP

using namespace std;
using namespace boost;

class TableState
{
private:
  mutex m_mutex;
  deque<vector<PoolBall> > m_balls;
  deque<cv::Mat> m_imgs;
  
  int m_historySize;
  bool m_fresh;
public:
  TableState(int historySize);
  
  void addState(cv::Mat img, vector<PoolBall> balls);
  bool getState(cv::Mat& img, vector<PoolBall>& balls, int age);
  bool getNewState(cv::Mat& img, vector<PoolBall>& balls);
};

#endif