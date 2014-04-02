#include "TableState.hpp"

TableState::TableState(int historySize)
{
  m_historySize = historySize;
  m_fresh = false;
}
  
void TableState::addState(Mat img, vector<PoolBall> balls)
{
  lock_guard<mutex> gg(m_mutex);
  
  m_imgs.push_front(img);
  m_balls.push_front(balls);
  
  if (m_imgs.size() > m_historySize)
  {
    m_imgs.pop_back();
  }
  
  if (m_balls.size() > m_historySize)
  {
    m_balls.pop_back();
  }
  
  m_fresh = true;
}

bool TableState::getState(Mat& img, vector<PoolBall>& balls, int age)
{
  lock_guard<mutex> gg(m_mutex);
  
  if (m_imgs.size() > age)
  {
    m_imgs[age].copyTo(img);
  }
  else
  {
    return false;
  }
  
  if (m_balls.size() > age)
  {
    balls = m_balls[age];
  }
  else
  {
    return false;
  }
  
  m_fresh = false;
}

bool TableState::getNewState(Mat& img, vector<PoolBall>& balls)
{
  if (m_fresh)
  {
    return getState(img, balls, 0);
  }
  
  return false;
}