#include "Process.hpp"

Process::Process() : m_run(false) {}

Process::~Process()
{
  stop();
  join();
}

//Caller requests copys of callee's outputs.
bool Process::requestInputs(list<pair<string, string> > requests, map<string, GenericPipe*> &storage)
{
  lock_guard<mutex> gg(m_outputLock);
  
  for (list<pair<string,string> >::iterator i = requests.begin(); i != requests.end(); i++)
  {
    
    if (m_outputs.count(i->first) == 1 && m_outputs[i->first]->ready())
    {
      storage[i->second] = m_outputs[i->first]->clone();
    }
    else
    {
      //Missing from output
      return false;
    }
  }
  
  return true;
}

//Ask designated input processes for copies of output
bool Process::getInputs()
{
  for (map<string, GenericPipe*>::iterator i = m_inputs.begin(); i != m_inputs.end(); i++)
  {
    delete i->second;
  }
  m_inputs.erase(m_inputs.begin(), m_inputs.end());
  
  for (map<Process*, list<pair<string, string> > >::iterator i = m_inputMap.begin(); i != m_inputMap.end(); i++)
  {
    if (!(i->first->requestInputs(i->second, m_inputs)))
    {
      return false;
    }
  }
  
  return true;
}

//Copy current wip into output buffer
void Process::setOutputs()
{
  lock_guard<mutex> gg(m_outputLock);
  
  // Delete current outputs
  for (map<string, GenericPipe*>::iterator i = m_outputs.begin(); i != m_outputs.end(); i++)
  {
    delete i->second;
  }
  m_outputs.erase(m_outputs.begin(), m_outputs.end());
  
  // Copy in wip and clear wip
  for (map<string, GenericPipe*>::iterator i = m_wip.begin(); i != m_wip.end(); i++)
  {
    m_outputs[i->first] = i->second->clone();
  }
  m_wip.erase(m_wip.begin(), m_wip.end());
  
}

//Start process thread
void Process::start(map<Process*, list<pair<string, string> > > inputs)
{
  m_inputMap = inputs;
  m_run = true;
  m_thread = thread(&Process::process, this);
}
void Process::start()
{
  m_run = true;
  m_thread = thread(&Process::process, this);
}

//Request thread stop
void Process::stop()
{
  m_run = false;
}

//Join thread. Should call stop first to avoid lockup.
void Process::join()
{
  m_thread.join();
}

//Process logic, calls init and then loops through in->work->out
void Process::process() 
{
  init();
  
  while (m_run)
  {
    if (getInputs())
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