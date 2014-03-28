#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <string>
#include <map>

using namespace cv;
using namespace std;

class PoolBall
{
private:
  Point m_center;
  int m_radius;

  Mat m_cropped;
  string m_type;
public:
  PoolBall(Mat img, Mat mask, Vec3f match);
  
  void markPosition(Mat img);
  void blankPosition(Mat img);
  bool isValid();
};