#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string>
#include <map>

#ifndef POOLBALL_HPP
#define POOLBALL_HPP

using namespace std;

class PoolBall
{
private:
  cv::Point m_center;
  int m_radius;
  bool m_valid;
  cv::Mat m_cropped;
  string m_type;
public:
  PoolBall(cv::Mat img, cv::Mat mask, cv::Vec3f match);
  
  void markPosition(cv::Mat img);
  void blankPosition(cv::Mat img);
  bool isValid();
};

#endif