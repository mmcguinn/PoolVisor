#include <list>
#include <map>
#include <string>
#include <boost/thread.hpp>
#include <sys/time.h>

#include "Pipe.hpp"

#ifndef PROCESS_HPP
#define PROCESS_HPP

using namespace std;
using namespace boost;

class Process
{
protected:
  virtual void init();
  virtual void work();
  
  bool getInputs();
  void setOutputs();
  void process();
  
  map<Process*, list<string> > m_inputMap;
  map<string, GenericPipe> m_inputs, m_wip, m_outputs;
  
  thread m_thread;
  mutex m_outputLock;
  bool m_run;
public:
  Process();
  ~Process();
  
  bool requestInputs(list<string> requests, map<string, GenericPipe> &storage);
  
  void start(map<Process*, list<string> > inputs);
  void stop();
  void join();
};

#endif