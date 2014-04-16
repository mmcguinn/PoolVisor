#include <cv.h>
#include <highgui.h>
#include <string>
#include <sys/time.h>

#include "Process.hpp"
#include "Pipe.hpp"

#ifndef INPUTPROCESS_HPP
#define INPUTPROCESS_HPP

class InputProcess : public Process
{
protected:
  cv::VideoCapture m_video;
  string m_src;
  int m_framesep;
public:
  InputProcess(string src, int framesep);
  
  void init();
  void work();
};

#endif
