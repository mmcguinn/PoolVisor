#include "StreamProc.hpp"

int timeval_subtract(timeval* result, timeval* x, timeval* y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) 
  {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) 
  {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
    tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

StreamProc::StreamProc(string dst, int width, int height)
{
  m_frameSize = height*width*3;
  
  struct v4l2_capability vidCaps;
  struct v4l2_format vidFormat;
  
  m_buffer = new uint8_t[m_frameSize];
  
  mode_t dc;
  m_fd = creat(dst.c_str(), dc);
  assert(m_fd >= 0);
  
  ioctl(m_fd, VIDIOC_QUERYCAP, &vidCaps);
  
  memset(&vidFormat, 0, sizeof(vidFormat));
  
  vidFormat.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  vidFormat.fmt.pix.height = height;
  vidFormat.fmt.pix.width = width;
  vidFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
  vidFormat.fmt.pix.sizeimage = m_frameSize;
  vidFormat.fmt.pix.field = V4L2_FIELD_NONE;
  vidFormat.fmt.pix.bytesperline = width*3;
  vidFormat.fmt.pix.colorspace = V4L2_COLORSPACE_JPEG;
  
  ioctl(m_fd, VIDIOC_S_FMT, &vidFormat);
}

void StreamProc::start()
{
  cout << "START - STREAMER" << endl;
  m_thread = thread(&StreamProc::process, this);
}

void StreamProc::join()
{
  m_thread.join();
}

void StreamProc::process()
{
  cout << "stream begin" << endl;
  timeval start, now, diff;
  gettimeofday(&start, NULL);
  
  while (1)
  {
    gettimeofday(&now, NULL);
    timeval_subtract(&diff, &now, &start); 
    if (diff.tv_usec + diff.tv_sec*1000000 > 1000000/30)
    {
      {
	lock_guard<mutex> gg(m_mutex);
	m_transmit.copyTo(m_nextFrame);
      }
      
      cout << diff.tv_sec << " - " << diff.tv_usec << endl;
      gettimeofday(&start, NULL);
      
      int i = 0;
      for (int x = 0; x < m_nextFrame.rows; x++)
      {
	for (int y = 0; y < m_nextFrame.cols; y++)
	{
	  cv::Vec3b px = m_nextFrame.at<cv::Vec3b>(x, y);
	  m_buffer[i++] = px[0];
	  m_buffer[i++] = px[1];
	  m_buffer[i++] = px[2];
	}
      }
      cout << ".";
      write(m_fd, m_buffer, m_frameSize);
    }
    else
    {
      usleep(100);
    }
  }
  
  close(m_fd);
}

void StreamProc::inputFrame(cv::Mat frame)
{
  lock_guard<mutex> gg(m_mutex);
  frame.copyTo(m_transmit);
}