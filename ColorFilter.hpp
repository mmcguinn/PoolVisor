#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace cv;
using namespace std;

class ColorFilter
{
private:
  Scalar m_low, m_high;
public:
  ColorFilter();
  ColorFilter(Scalar low, Scalar high);
  
  static map<string, ColorFilter> load(string filename);
  
  Mat makeMask(Mat input, bool invert);
  
  int countMask(Mat input);
  
  void test();
};

int colorThreshold(Mat src, Mat dst, Scalar low, Scalar high);