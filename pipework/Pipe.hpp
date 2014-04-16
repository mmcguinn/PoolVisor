#include <cv.h>

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

class GenericPipe
{
public:
  GenericPipe() : m_ready(false) {}
  
  bool ready() 
  {
    return m_ready;
  }
  
  virtual GenericPipe* clone() {};
  
protected:
  bool m_ready;
};

template <typename T>
class Pipe : public GenericPipe
{
protected:
  T m_val;
  
public:
  Pipe() {}
  
  bool get(T& slot)
  {
    if (m_ready())
    {
      slot = m_val;
    }
    return m_ready;
  }
  
  void set(T& input)
  {
    m_val = input;
    m_ready = true;
  }
  
  Pipe<T>* clone()
  {
    Pipe<T> *cpy = new Pipe<T>();
    if (ready())
    {
      cpy->set(m_val);
    }
  }
};

template <>
class Pipe<cv::Mat> : public GenericPipe
{
protected:
  cv::Mat m_val;
  
public:
  Pipe() {}
  
  bool get(cv::Mat& slot)
  {
    if (ready())
    {
      m_val.copyTo(slot);
    }
    return ready();
  }
  
  void set(cv::Mat& input)
  {
    input.copyTo(m_val);
    m_ready = true;
  }
  
  Pipe<cv::Mat>* clone()
  {
    Pipe<cv::Mat>* cpy = new Pipe<cv::Mat>;
    if (ready())
    {
      cpy->set(m_val);
    }
  }
};

#endif