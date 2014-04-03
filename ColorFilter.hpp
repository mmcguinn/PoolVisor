#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#ifndef COLORFILTER_HPP
#define COLORFILTER_HPP

using namespace std;

class ColorFilter
{
private:
  cv::Scalar m_low, m_high;
public:
  ColorFilter();
  ColorFilter(cv::Scalar low, cv::Scalar high);
  
  static map<string, ColorFilter> load(string filename);
  
  cv::Mat makeMask(cv::Mat input, bool invert);
  
  int countMask(cv::Mat input);
  
  void test();
};

int colorThreshold(cv::Mat src, cv::Mat dst, cv::Scalar low, cv::Scalar high);

#endif