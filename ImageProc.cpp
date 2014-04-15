#include "ImageProc.hpp"

void medianShift(cv::Mat& src, cv::Mat& dst,
		 cv::Scalar currentMedian, cv::Scalar targetMedian)
{
  src.copyTo(dst);
  cv::Vec3f normCM;
  for(int i = 0; i < 3; i++)
  {
    normCM[i] = log(targetMedian[i] / 255.0f) / log(currentMedian[i] / 255.0f);
  }
  for(int j=0; j<src.cols; j++) 
  {
    for(int i=0; i<src.rows; i++) 
    {
      for(int c=0; c<3; c++) 
      {
	dst.at<cv::Vec3b>(i,j)[c] = (uint8_t)(pow(src.at<cv::Vec3b>(i,j)[c]/255.0f, normCM[c])*255);
      }
    }
  }
}

ImageProc::ImageProc(string source, TableState& state) : m_state(state)
{
  m_feltFilter = ColorFilter::load("table.filters")["felt"];
  m_ballFilters = ColorFilter::load("balls.filters");
  //m_video.open(source);
  m_running = false;
  
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
  cout << "START - IMAGE" << endl;
  m_thread = thread(&ImageProc::process, this);
}

void ImageProc::join()
{
  m_thread.join();
}

void ImageProc::process()
{
  cout << "image begin" << endl;
  cv::Mat img;
  
//   m_readThread = thread(&ImageProc::frameSkipper, this);
//   
//   while (!m_running)
//   {
//     sleep(1);
//   }
  
  while (1)
  {
//     {
//       lock_guard<mutex> gg(m_readMutex);
//       m_nextFrame.copyTo(img);
//     }
    img = cv::imread("pool1.jpg");
    // --- //
    //static int frameNum = 0;
    //char filename[256];
    //frameNum++;
    //sprintf(filename, "dump/before%02d.jpg", frameNum);
    cv::imwrite("Pre-WB.jpg", img);
    medianShift(img, img, cv::mean(img), cv::Scalar(120,120,120));
    //sprintf(filename, "dump/after%02d.jpg", frameNum);
    cv::imwrite("Post-WB.jpg", img);
    // --- //

    //m_state.addState(img, lame);
    //continue;

    cv::Mat imgcpy(img), rawBalls, greyBalls;
    cv::cvtColor(img,img, CV_BGR2HSV);
  
    cv::Mat feltMask = m_feltFilter.makeMask(img, true);
    
    cv::cvtColor(feltMask, feltMask, CV_GRAY2BGR);
    cv::cvtColor(img,img, CV_HSV2BGR);
    
    cv::bitwise_and(img, feltMask, rawBalls);
    
    cv::cvtColor(feltMask, greyBalls, CV_BGR2GRAY);
    cv::cvtColor(feltMask, feltMask, CV_BGR2GRAY);
    
    vector<PoolBall> balls, newBalls;
    do
    {
      newBalls = findBalls(img, greyBalls);
      
      cv::Mat tmp = rawBalls.clone();
      for (vector<PoolBall>::iterator i = newBalls.begin(); i != newBalls.end(); i++)
      {
	balls.push_back(*i);
	i->blankPosition(greyBalls);
	
	i->markPosition(tmp);
      }
      
    } while (newBalls.size() > 0);
    
    //Display
    for (vector<PoolBall>::iterator i = balls.begin(); i != balls.end(); i++)
    {
      i->markPosition(imgcpy);
    }
    
    cv::imwrite("find.jpg", imgcpy);
    cv::imshow("ggg", imgcpy);
    cv::waitKey(0);
    //m_state.addState(img, balls);
  }
  
  
  //Do something
}

vector<PoolBall> ImageProc::findBalls(cv::Mat img, cv::Mat oriMask)
{
  cv::imwrite("Pre-OPEN.jpg", oriMask);
  cv::Mat mask = oriMask.clone();
  cv::morphologyEx(mask, mask, cv::MORPH_OPEN, 
		   getStructuringElement(cv::MORPH_ELLIPSE, 
					 cv::Size(m_conf["noise_floor"], 
						  m_conf["noise_floor"])));
  cv::imwrite("Post-OPEN.jpg", mask);
  cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, 
		   getStructuringElement(cv::MORPH_ELLIPSE, 
					 cv::Size(m_conf["fill_width"], 
						  m_conf["fill_width"])));
  cv::imwrite("Post-CLOSE.jpg", mask);
  cv::morphologyEx(mask, mask, cv::MORPH_OPEN, 
		   getStructuringElement(cv::MORPH_ELLIPSE, 
					 cv::Size(m_conf["exp_ball_width"], 
						  m_conf["exp_ball_width"])));
  cv::imwrite("Post-OPEN2.jpg", mask);
  cv::GaussianBlur(mask, mask, cv::Size(9, 9), 2, 2 );
  
//   static int frameNum = 0;
//   frameNum++;
//   char filename[256];
//   sprintf(filename, "dump/mask%02d.jpg", frameNum);
//   cv::imwrite(filename, mask);
//   sprintf(filename, "dump/img%02d.jpg", frameNum);
//   cv::imwrite(filename, img);
  
  vector<PoolBall> results;
  vector<cv::Vec3f> circles;
  cv::HoughCircles(mask, circles, CV_HOUGH_GRADIENT, 3, m_conf["min_ball_radius"], 200, 60, 
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

void ImageProc::frameSkipper()
{
  bool run = true;
  while (run)
  {
    {
      lock_guard<mutex> gg(m_readMutex);
      run = m_video.read(m_nextFrame);
    }
    m_running = true;
    
    usleep(1000);
  }
}
