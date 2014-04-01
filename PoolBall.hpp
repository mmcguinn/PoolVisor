#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string>
#include <map>

#ifndef POOLBALL_HPP
#define POOLBALL_HPP

using namespace cv;
using namespace std;

class PoolBall
{
private:
  Point m_center;
  int m_radius;
  bool m_valid;
  Mat m_cropped;
  string m_type;
public:
  PoolBall(Mat img, Mat mask, Vec3f match);
  
  void markPosition(Mat img);
  void blankPosition(Mat img);
  bool isValid();
};

#endif