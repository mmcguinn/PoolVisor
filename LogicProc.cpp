#include "LogicProc.hpp"

LogicProc::LogicProc(string dst, TableState& state) : m_state(state)
{
  m_dst = dst;
}

void LogicProc::start()
{
  cout << "START - LOGIC" << endl;
  m_thread = thread(&LogicProc::process, this);
}

void LogicProc::join()
{
  m_thread.join();
}

void LogicProc::process()
{
  cout << "logic begin" << endl;
  cv::Mat curFrame;
  vector<PoolBall> curBalls;
  
  while (!m_state.getNewState(curFrame, curBalls))
  {
    usleep(100);
  }
    
  StreamProc streamer(m_dst, curFrame.cols, curFrame.rows);
  streamer.inputFrame(curFrame);
  streamer.start();
  
  while (1)
  {
    while (!m_state.getNewState(curFrame, curBalls))
    {
      usleep(100);
    }
    
    streamer.inputFrame(curFrame);
  }
}