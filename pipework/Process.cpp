#include "Process.hpp"

Process::Process() : m_run(false) {}

Process::~Process()
{
  stop();
  join();
}

bool Process::requestInputs(list<string> requests, map<string, GenericPipe> &storage)
{
  lock_guard<mutex> gg(m_outputLock);
  
  for (list<string>::iterator i = requests.begin(); i != requests.end(); i++)
  {
    
    if (m_outputs.count(*i) == 1 && m_outputs[*i].ready())
    {
      storage[*i] = m_outputs[*i].clone();
    }
    else
    {
      //Missing from output
      return false;
    }
  }
  
  return true;
}

bool Process::getInputs()
{
  m_inputs.erase(m_inputs.begin(), m_inputs.end());
  
  for (map<Process&, list<string> >::iterator i = m_inputMap->begin(); i != m_inputMap->end(); i++)
  {
    if (!(i->first->requestInputs(i->second, m_inputs)))
    {
      return false;
    }
  }
  
  return true;
}

void Process::setOutputs()
{
  lock_guard<mutex> gg(m_outputLock);
  m_output.erase(m_output.begin(), m_output.end());
  
  for (map<string, GenericPipe>::iterator i = m_wip.begin(); i != m_wip.end(); i++)
  {
    m_output[i->first] = i->second.clone();
  }
  
  m_wip.erase(m_wip.begin(), m_wip.end());
  
}

void Process::start(map<GenericPipe*, list<string> > inputs)
{
  m_inputMap = inputs;
  m_run = true;
  m_thread = thread(&Process::process, this);
}

void Process::stop()
{
  m_run = false;
}

void Process::join()
{
  m_thread.join();
}

Process::process() 
{
  init();
  
  while (m_run)
  {
    if (getInputs(m_inputMap))
    {
      work();
      setOutputs();
    }
    else
    {
      //Don't spin
      usleep(100);
    }
  }
}