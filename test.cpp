#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace cv;
using namespace std;

//g++ -o PoolVisor test.cpp  `pkg-config --cflags --libs opencv`

int colorThreshold(Mat src, Mat dst, Scalar low, Scalar high)
{
    bool wrap = low.val[0] > high.val[0];
    int ret = 0;

    for(int row = 0; row < src.rows; ++row) 
    {
        uchar * p_src = src.ptr<uchar>(row);
        uchar * p_dst = dst.ptr<uchar>(row);
        for (int col = 0; col < src.cols; ++col) 
        {
            int srcH = *p_src++;
            int srcS = *p_src++;
            int srcV = *p_src++;

            int clr = 0;
            
            if (wrap)
            {
              if((srcH >= low.val[0] || srcH <= high.val[0]) &&
                  srcS >= low.val[1] && srcS <= high.val[1] &&
                  srcV >= low.val[2] && srcV <= high.val[2]) 
              {
                  clr = 255;
                  ret++;
              }
            }
            else if(srcH >= low.val[0] && srcH <= high.val[0] &&
                    srcS >= low.val[1] && srcS <= high.val[1] &&
                    srcV >= low.val[2] && srcV <= high.val[2]) 
            {
                clr = 255;
                ret++;
            }
              
            *p_dst++ = clr;
        }
    }
    
    return ret;
}

class ColorFilter
{
private:
  Scalar m_low, m_high;
public:
  ColorFilter() {}
  ColorFilter(Scalar low, Scalar high)
  {
    m_low = low;
    m_high = high;
  }
  
  static map<string, ColorFilter> load(string filename)
  {
    ifstream input(filename.c_str(), ifstream::in);
    
    map<string, ColorFilter> result;
    string name, dc;
    int h1,s1,v1,h2,s2,v2, rotation;
    
    while (input.good())
    {
      input >> name >> h1 >> s1 >> v1 >> dc >> h2 >> s2 >> v2;
      result[name] = ColorFilter(Scalar(h1,s1,v1), Scalar(h2,s2,v2));
    }
    
    return result;
  }
  
  Mat makeMask(Mat input, bool invert = false)
  {
    Mat result = Mat::zeros( input.rows, input.cols, CV_8UC1 );
    
    colorThreshold(input, result, m_low, m_high);
                   
    if (invert)
    {
        bitwise_not(result, result);
    }
    
    return result;
  }
  
  int countMask(Mat input)
  {
    Mat result = Mat::zeros( input.rows, input.cols, CV_8UC1 );
    
    return colorThreshold(input, result, m_low, m_high);
  }
  
  void test()
  {
    cout << m_low << "==" << m_high << endl;
  }
};

int h1,s1,v1,h2,s2,v2;

void updateTest(int, void*)
{
  Mat img;
  if(!(img=imread("pool1.jpg", 1)).data)
  {
      return;
  }
  cvtColor(img,img, CV_BGR2HSV);
  ColorFilter filter(Scalar(h1,s1,v1), Scalar(h2,s2,v2));
  Mat mask = filter.makeMask(img, false);
  cvtColor(mask, mask, CV_GRAY2BGR);
  bitwise_and(img, mask, img);
  cvtColor(img,img, CV_HSV2BGR);
  imshow("panel", img);
}

void startTest()
{
  Mat img;
  if(!(img=imread("pool1.jpg", 1)).data)
  {
      return;
  }
  
  imshow("panel", img);
  
  namedWindow("slider", 1);
  createTrackbar("h1", "slider", &h1, 180, updateTest);
  createTrackbar("s1", "slider", &s1, 255, updateTest);
  createTrackbar("v1", "slider", &v1, 255, updateTest);
  createTrackbar("h2", "slider", &h2, 180, updateTest);
  createTrackbar("s2", "slider", &s2, 255, updateTest);
  createTrackbar("v2", "slider", &v2, 255, updateTest);
  waitKey(0);
}


int main(int argc, char* argv[])
{
  Mat img, greyBalls, rawBalls;
  
  if(!(img=imread("pool1.jpg", 1)).data)
  {
      return -1;
  }
  
  ColorFilter feltFilter = ColorFilter::load("table.filters")["felt"];
  map<string, ColorFilter> ballFilters = ColorFilter::load("balls.filters");
  
  cvtColor(img,img, CV_BGR2HSV);
  
  Mat feltMask = feltFilter.makeMask(img, true);
  
  cvtColor(feltMask, feltMask, CV_GRAY2BGR);
  cvtColor(img,img, CV_HSV2BGR);
  
  bitwise_and(img, feltMask, rawBalls);
  
  
  cvtColor(feltMask, greyBalls, CV_BGR2GRAY);
  cvtColor(feltMask, feltMask, CV_BGR2GRAY);
  
  // smooth it, otherwise a lot of false circles may be detected
  GaussianBlur( greyBalls, greyBalls, Size(25, 25), 9, 9 );
  threshold(greyBalls, greyBalls, 40, 255, THRESH_BINARY);
  GaussianBlur( greyBalls, greyBalls, Size(9, 9), 2, 2 );
  
  vector<Vec3f> circles;
  HoughCircles(greyBalls, circles, CV_HOUGH_GRADIENT,
                 2, 40, 200, 60, 10, 50 );

  for( size_t i = 0; i < circles.size(); i++ )
    {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // draw the circle center
      circle( rawBalls, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // draw the circle outline
      circle( rawBalls, center, radius, Scalar(0,0,255), 3, 8, 0 );
	 
      // center and radius are the results of HoughCircle
      // mask is a CV_8UC1 image with 0
      
      Mat ball, ballMask = Mat::zeros( img.rows, img.cols, CV_8UC1 );
      circle( ballMask, center, radius, Scalar(255,255,255), -1, 8, 0 ); //-1 means filled
      bitwise_and(feltMask, ballMask, ballMask);
      img.copyTo( ball, ballMask ); // copy values of img to dst if mask is > 0.
      Mat ballCropped( ball, Rect( center.x-radius, center.y-radius, radius*2, radius*2 ) );
      
      int max = 0;
      string type;
      map<String, int> colorResults;
      cvtColor(ballCropped, ballCropped, CV_BGR2HSV);
      cout << endl << endl << endl;
      for (map<string, ColorFilter>::iterator i = ballFilters.begin(); i != ballFilters.end(); ++i)
      {
        colorResults[i->first] = i->second.countMask(ballCropped);
        cout << i->first << " - " << colorResults[i->first] << endl;
        
        if (i->first != "white" && colorResults[i->first] > max)
        {
          max = colorResults[i->first];
          type = i->first;
        }
      }
      
      if (colorResults["white"] == 0)
      {
        type = "solid " + type;
        cout << "solid " << type << endl;
      }
      else if ((colorResults["white"]) / (float)(max + colorResults["white"]) >= .9)
      {
        type = "cue";
        cout << "cue" << endl;
      }
      else if ((colorResults["white"]) / (float)(max + colorResults["white"]) >= .175)
      {
        type = "striped " + type;
        cout << "striped " << type << endl;
      }
      else
      {
        type = "solid " + type;
        cout << "solid " << type << endl;
      }
      
      cvtColor(ballCropped, ballCropped, CV_HSV2BGR);

      Point textStart(center.x - 50, center.y - 40);
      putText(rawBalls, type.c_str(), textStart, 
             FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
      
      imshow("test", ballCropped);
      waitKey(0);
    }
    
    imshow("img", img);
    imshow("gray", greyBalls);
    imshow("anded", rawBalls);
    
    waitKey(0);
    
    startTest();
    return 0;
}
