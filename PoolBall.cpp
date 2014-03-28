#include "PoolBall.hpp"
#include "ColorFilter.hpp"

map<string, ColorFilter> ballFilters = ColorFilter::load("balls.filters");

PoolBall::PoolBall(Mat img, Mat mask, Vec3f match)
{
  m_center = Point(cvRound(match[0]), cvRound(match[1]));
  m_radius = cvRound(match[2]);
  
  Mat ball, ballMask = Mat::zeros( img.rows, img.cols, CV_8UC1 );
  //Mask circle at ball location
  circle(ballMask, m_center, m_radius, Scalar(255,255,255), -1, 8, 0); //-1 means filled
  //Take from img at mask
  img.copyTo(ball, ballMask); // copy values of img to dst if mask is > 0.
  //Crop ball image
  m_cropped = Mat(ball, Rect(m_center.x-m_radius, m_center.y-m_radius, m_radius*2, m_radius*2));

  int max = 0;
  map<String, int> colorResults;
  cvtColor(m_cropped, m_cropped, CV_BGR2HSV);
  cout << endl << endl << endl;
  for (map<string, ColorFilter>::iterator i = ballFilters.begin(); i != ballFilters.end(); ++i)
  {
    colorResults[i->first] = i->second.countMask(m_cropped);
    cout << i->first << " - " << colorResults[i->first] << endl;
    
    if (i->first != "white" && colorResults[i->first] > max)
    {
      max = colorResults[i->first];
      m_type = i->first;
    }
  }

  if (colorResults["white"] == 0)
  {
    m_type = "solid " + m_type;
  }
  else if ((colorResults["white"]) / (float)(max + colorResults["white"]) >= .9)
  {
    m_type = "cue";
  }
  else if ((colorResults["white"]) / (float)(max + colorResults["white"]) >= .175)
  {
    m_type = "striped " + m_type;
  }
  else
  {
    m_type = "solid " + m_type;
  }
}
  
void PoolBall::markPosition(Mat img)
{
  if (isValid())
  {
    circle(img, m_center, 3, Scalar(0,255,0), -1);
    circle(img, m_center, m_radius, Scalar(0,0,255), 3);
  }
}

void PoolBall::blankPosition(Mat img)
{
  if (isValid())
  {
    circle(img, m_center, m_radius+3, Scalar(0,0,0), -1);
  }
}

bool PoolBall::isValid()
{
  return true;
}
