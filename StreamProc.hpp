#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string>
#include <map>
#include <boost/thread.hpp>

#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <cstdlib>
#include <sys/time.h>

#ifndef STREAMPROC_HPP
#define STREAMPROC_HPP

using namespace std;
using namespace boost;

class StreamProc
{
private:
  thread m_thread;
  mutex m_mutex;
  cv::Mat m_transmit, m_nextFrame;
  bool m_newFrame;
  int m_fd, m_frameSize;
  uint8_t *m_buffer;
public:
  StreamProc(string dst, int height, int width);
  
  void start();
  void join();
  void process();

  void inputFrame(cv::Mat frame);
};

#endif