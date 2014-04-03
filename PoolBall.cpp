#include "PoolBall.hpp"
#include "ColorFilter.hpp"

map<string, ColorFilter> ballFilters = ColorFilter::load("balls.filters");

PoolBall::PoolBall(cv::Mat img, cv::Mat mask, cv::Vec3f match)
{
  m_center = cv::Point(cvRound(match[0]), cvRound(match[1]));
  m_radius = cvRound(match[2]);
  
  // Verify ball is positive in mask
  if ((int)mask.at<uchar>(m_center.y, m_center.x) == 0)
  {
    m_valid = false;
    return;
  }
  else
  {
    m_valid = true;
  }
  
  cv::Mat ball, ballMask = cv::Mat::zeros( img.rows, img.cols, CV_8UC1 );
  //Mask circle at ball location
  cv::circle(ballMask, m_center, m_radius, cv::Scalar(255,255,255), -1, 8, 0); //-1 means filled
  //Take from img at mask
  img.copyTo(ball, ballMask); // copy values of img to dst if mask is > 0.
  //Crop ball image
  int x = m_center.x-m_radius >= 0 ? m_center.x-m_radius : 0;
  int y = m_center.y-m_radius >= 0 ? m_center.y-m_radius : 0;
  int width = x+m_radius*2 < ball.cols ? m_radius*2 : ball.cols-x;
  int height = y+m_radius*2 < ball.rows ? m_radius*2 : ball.rows-y;
//   cout << x << "," << y << "," << width << "," << height << endl;
//   cout << ball.cols << "," << ball.rows << endl;
  m_cropped = cv::Mat(ball, cv::Rect(x, y, width, height));

  int max = 0;
  map<string, int> colorResults;
  cv::cvtColor(m_cropped, m_cropped, CV_BGR2HSV);
  //cout << endl << endl << endl;
  for (map<string, ColorFilter>::iterator i = ballFilters.begin(); i != ballFilters.end(); ++i)
  {
    colorResults[i->first] = i->second.countMask(m_cropped);
    //cout << i->first << " - " << colorResults[i->first] << endl;
    
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
  
void PoolBall::markPosition(cv::Mat img)
{
  if (isValid())
  {
    cv::circle(img, m_center, 3, cv::Scalar(0,255,0), -1);
    cv::circle(img, m_center, m_radius, cv::Scalar(0,0,255), 3);
    
    cv::Point textStart(m_center.x - 50, m_center.y - 40);
    cv::putText(img, m_type.c_str(), textStart, cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(255,255,255), 1, CV_AA);
  }
}

void PoolBall::blankPosition(cv::Mat img)
{
  if (isValid())
  {
    cv::circle(img, m_center, m_radius+3, cv::Scalar(0,0,0), -1);
  }
}

bool PoolBall::isValid()
{
  return m_valid;
}
