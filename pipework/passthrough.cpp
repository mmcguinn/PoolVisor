#include "InputProcess.hpp"
#include "V4L2LoopbackProcess.hpp"

int main(int argc, char** argv)
{
  InputProcess inpt("/home/michael/Downloads/shortgameclip.h264", 1000000/30);
  V4L2LoopbackProcess strm("/dev/video0", 720, 1280, 30);
  
  pair<string,string> mapping("img", "img");
  list<pair<string,string> > lst;
  lst.push_back(mapping);
  map<Process*, list<pair<string,string> > > strmMapping;
  strmMapping[&inpt] = lst;
  
  inpt.start();
  strm.start(strmMapping);
  
  inpt.join();
  strm.join();
}