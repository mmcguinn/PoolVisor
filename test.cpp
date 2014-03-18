#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>

using namespace cv;

void showHistogram(Mat& img);

int main(int argc, char* argv[])
{
  Mat img, gray, post, anded;
  
  if(!(img=imread("pool1.jpg", 1)).data)
  {
      std::cout << "crap" << std::endl;
      return -1;
  }
  else
  {
      std::cout << "yay" << std::endl;
  }
  
  cvtColor(img,img, CV_BGR2HSV);
  inRange(img, Scalar(0, 0, 60), Scalar(180, 100, 200), post);
  
  bitwise_not(post, post);
  
  cvtColor(post,post, CV_GRAY2BGR);
  cvtColor(img,img, CV_HSV2BGR);
  
  bitwise_and(img, post, anded);
  
  
  cvtColor(post, gray, CV_BGR2GRAY);
  cvtColor(post, post, CV_BGR2GRAY);
  
  // smooth it, otherwise a lot of false circles may be detected
  GaussianBlur( gray, gray, Size(25, 25), 9, 9 );
  threshold(gray, gray, 40, 255, THRESH_BINARY);
  GaussianBlur( gray, gray, Size(9, 9), 2, 2 );
  
  vector<Vec3f> circles;
  HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
                 2, 40, 200, 60, 10, 50 );

  for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( anded, center, 3, Scalar(0,255,0), -1, 8, 0 );
         // draw the circle outline
         circle( anded, center, radius, Scalar(0,0,255), 3, 8, 0 );
	 
	 // center and radius are the results of HoughCircle
	// mask is a CV_8UC1 image with 0
	
	Mat dst, mask = Mat::zeros( img.rows, img.cols, CV_8UC1 );
	circle( mask, center, radius, Scalar(255,255,255), -1, 8, 0 ); //-1 means filled
	bitwise_and(post, mask, mask);
	img.copyTo( dst, mask ); // copy values of img to dst if mask is > 0.
	Mat cropped( dst, Rect( center.x-radius, center.y-radius, radius*2, radius*2 ) );
	
	imshow("test", cropped);
	showHistogram(cropped);
	waitKey(0);
    }
    
//     imshow("img", img);
//     imshow("gray", gray);
//     imshow("anded", anded);
//     showHistogram(img);
//     waitKey(0);
//     imwrite( "post.jpg", post );
//     
//     imwrite( "anded.jpg", anded );
//     
//     imwrite( "gray.jpg", gray );
//     
//     imwrite( "img.jpg", img );
  
    return 0;
}

void showHistogram(Mat& img)
{
	int bins = 256;             // number of bins
	int nc = img.channels();    // number of channels

	vector<Mat> hist(nc);       // histogram arrays

	// Initalize histogram arrays
	for (int i = 0; i < hist.size(); i++)
		hist[i] = Mat::zeros(1, bins, CV_32SC1);

	// Calculate the histogram of the image
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (!((img.at<Vec3b>(i,j)[0] == 0) && 
			      (img.at<Vec3b>(i,j)[1] == 0) && 
			      (img.at<Vec3b>(i,j)[2] == 0)))
			{
			for (int k = 0; k < nc; k++)
			{
				uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
				hist[k].at<int>(val) += 1;
			}
			}
		}
	}
	

	// For each histogram arrays, obtain the maximum (peak) value
	// Needed to normalize the display later
	int hmax = 0;
	for (int i = 0; i < nc; i++)
	{
		for (int j = 0; j < bins-1; j++)
			hmax = hist[i].at<int>(j) > hmax ? hist[i].at<int>(j) : hmax;
	}

	const char* wname[3] = { "blue", "green", "red" };
	Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

	vector<Mat> canvas(nc);

	// Display each histogram in a canvas
	for (int i = 0; i < nc; i++)
	{
		canvas[i] = Mat::ones(125, bins, CV_8UC3);

		for (int j = 0, rows = canvas[i].rows; j < bins-1; j++)
		{
			line(
				canvas[i], 
				Point(j, rows), 
				Point(j, rows - (hist[i].at<int>(j) * rows/hmax)), 
				nc == 1 ? Scalar(200,200,200) : colors[i], 
				1, 8, 0
			);
		}

		imshow(nc == 1 ? "value" : wname[i], canvas[i]);
	}
}