#include "InputProcess.hpp"

InputProcess::InputProcess(string src, int framesep = 0)
{
  m_framesep = framesep;
  m_src = src;
}
  
void InputProcess::init()
{
  m_video.open(m_src);
}

void InputProcess::work()
{
  cv::Mat frame;
  
  usleep(m_framesep);
  
  if (m_video.read(frame))
  {
    Pipe<cv::Mat> *img = new Pipe<cv::Mat>;
    img->set(frame);
    m_wip["img"] = img;
  }
  else
  {
    m_run = false;
  }
}