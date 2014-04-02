#include <cv.h>
#include <math.h>
#include <string>
#include <map>
#include <deque>
#include <boost/thread.hpp>

#include "PoolBall.hpp"

#ifndef TABLESTATE_HPP
#define TABLESTATE_HPP

using namespace cv;
using namespace std;
using namespace boost;

class TableState
{
private:
  mutex m_mutex;
  deque<vector<PoolBall> > m_balls;
  deque<Mat> m_imgs;
  
  int m_historySize;
  bool m_fresh;
public:
  TableState(int historySize);
  
  void addState(Mat img, vector<PoolBall> balls);
  bool getState(Mat& img, vector<PoolBall>& balls, int age);
  bool getNewState(Mat& img, vector<PoolBall>& balls);
};

#endif