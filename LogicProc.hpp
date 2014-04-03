#include <cv.h>
#include <math.h>
#include <string>
#include <map>
#include <boost/thread.hpp>
#include <sys/time.h>

#include "PoolBall.hpp"
#include "TableState.hpp"
#include "StreamProc.hpp"

#ifndef LOGICPROC_HPP
#define LOGICPROC_HPP

using namespace std;
using namespace boost;

class LogicProc
{
private:
  thread m_thread;
  string m_dst;
  TableState& m_state;
public:
  LogicProc(string dst, TableState& state);
  
  void start();
  void join();
  void process();
};

#endif