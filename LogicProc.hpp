#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string>
#include <map>
#include <boost/thread.hpp>

#include "PoolBall.hpp"
#include "TableState.hpp"

#ifndef LOGICPROC_HPP
#define LOGICPROC_HPP

using namespace cv;
using namespace std;
using namespace boost;

class LogicProc
{
private:
  thread m_thread;
  TableState& m_state;
public:
  LogicProc(string dst, TableState& state);
  
  void start();
  void join();
  void process();
};

#endif