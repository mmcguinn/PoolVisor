#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <cstdlib>
#include <sys/time.h>
#include <cv.h>

#include "Process.hpp"
#include "Pipe.hpp"

#ifndef V4L2LOOPBACKPROCESS_HPP
#define V4L2LOOPBACKPROCESS_HPP

class V4L2LoopbackProcess : public Process
{
protected:
  int m_framerate, m_fd, m_frameSize;
  timeval m_start;
  uint8_t *m_buffer;
public:
  V4L2LoopbackProcess(string dst, int height, int width, int framerate);
  
  void init();
  void work();
};


#endif
