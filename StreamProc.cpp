#include "StreamProc.hpp"

StreamProc::StreamProc(string dst, int width, int height)
{
  m_frameSize = height*width*3;
  
  struct v4l2_capability vidCaps;
  struct v4l2_format vidFormat;
  
  m_buffer new[] __u8[m_frameSize];
  
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

void StreamProc::process()
{
  
  
  while (1)
  {
    for (int i = 0; i < m_frameSize; i++)
    {
      m_buffer[i] = rand()%255;
    }
    write(m_fd, m_buffer, m_frameSize);
  }
  
  close(m_fd);
}