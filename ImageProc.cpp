#include "ImageProc.hpp"

ImageProc::ImageProc(string source, TableState& state) : m_state(state)
{
  m_feltFilter = ColorFilter::load("table.filters")["felt"];
  m_ballFilters = ColorFilter::load("balls.filters");
  m_video.open(source);
  
  ifstream input("imageproc.cfg", ifstream::in);
  
  string name;
  int val;
  
  while (input.good())
  {
    input >> name >> val;
    m_conf[name] = val;
  }
}
  
void ImageProc::start()
{
  m_thread = thread(&ImageProc::process, this);
}

void ImageProc::join()
{
  m_thread.join();
}

void ImageProc::process()
{
  Mat img;
  
  while (m_video.read(img))
  {
    Mat imgcpy(img), rawBalls, greyBalls;
    cvtColor(img,img, CV_BGR2HSV);
  
    Mat feltMask = m_feltFilter.makeMask(img, true);
    
    cvtColor(feltMask, feltMask, CV_GRAY2BGR);
    cvtColor(img,img, CV_HSV2BGR);
    
    bitwise_and(img, feltMask, rawBalls);
    
    cvtColor(feltMask, greyBalls, CV_BGR2GRAY);
    cvtColor(feltMask, feltMask, CV_BGR2GRAY);
    
    vector<PoolBall> balls, newBalls;
    do
    {
      newBalls = findBalls(img, greyBalls);
      
      Mat tmp = rawBalls.clone();
      for (vector<PoolBall>::iterator i = newBalls.begin(); i != newBalls.end(); i++)
      {
	balls.push_back(*i);
	i->blankPosition(greyBalls);
	
	i->markPosition(tmp);
      }
      
      imshow("anded", tmp);
      waitKey(0);
      
    } while (newBalls.size() > 0);
    
    //Display
    for (vector<PoolBall>::iterator i = balls.begin(); i != balls.end(); i++)
    {
      i->markPosition(imgcpy);
    }
    imshow("anded", imgcpy);
    waitKey(0);
  }
  
  
  //Do something
}

vector<PoolBall> ImageProc::findBalls(Mat img, Mat oriMask)
{
  Mat mask = oriMask.clone();
  morphologyEx(mask, mask, MORPH_OPEN, 
	       getStructuringElement(MORPH_ELLIPSE, cvSize(m_conf["noise_floor"],
							   m_conf["noise_floor"])));
  morphologyEx(mask, mask, MORPH_CLOSE, 
	       getStructuringElement(MORPH_ELLIPSE, cvSize(m_conf["fill_width"],
							   m_conf["fill_width"])));
  morphologyEx(mask, mask, MORPH_OPEN, 
	       getStructuringElement(MORPH_ELLIPSE, cvSize(m_conf["exp_ball_width"],
							   m_conf["exp_ball_width"])));
  GaussianBlur(mask, mask, Size(9, 9), 2, 2 );
  
  imshow("gray", mask);
  
  vector<PoolBall> results;
  vector<Vec3f> circles;
  HoughCircles(mask, circles, CV_HOUGH_GRADIENT, 2, m_conf["min_ball_radius"], 200, 60, 
	       m_conf["min_ball_radius"], m_conf["max_ball_radius"]);
  
  for( size_t i = 0; i < circles.size(); i++)
  {
    PoolBall ball(img, mask, circles[i]);
    if (ball.isValid())
    {
      results.push_back(ball);
    }
  }
  
  return results;
}