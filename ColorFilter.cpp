#include "ColorFilter.hpp"

ColorFilter::ColorFilter() {}

ColorFilter::ColorFilter(Scalar low, Scalar high)
{
  m_low = low;
  m_high = high;
}

map<string, ColorFilter> ColorFilter::load(string filename)
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

Mat ColorFilter::makeMask(Mat input, bool invert = false)
{
  Mat result = Mat::zeros( input.rows, input.cols, CV_8UC1 );
  
  colorThreshold(input, result, m_low, m_high);
		  
  if (invert)
  {
      bitwise_not(result, result);
  }
  
  return result;
}

int ColorFilter::countMask(Mat input)
{
  Mat result = Mat::zeros( input.rows, input.cols, CV_8UC1 );
  
  return colorThreshold(input, result, m_low, m_high);
}

void ColorFilter::test()
{
  cout << m_low << "==" << m_high << endl;
}


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
