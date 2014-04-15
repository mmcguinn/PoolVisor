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
    
    if (m_output.count(*i) == 1 && m_output[*i].ready())
    {
      storage[*i] = m_output[*i].clone();
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
  m_input.erase(m_input.begin(), m_input.end());
  
  for (map<Process&, list<string> >::iterator i = m_inputMap.begin(); i != m_inputMap.end(); i++)
  {
    if (!(i->first.requestInputs(i->second, m_input)))
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

void Process::start(map<Pipe&, list<string> > inputs)
{
  m_inputs = inputs;
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