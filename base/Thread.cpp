#include "base/Thread.h"
#include "base/CurrentThread.h"

namespace base
{
namespace detail
{
  struct  ThreadData
  {
    typedef base::Thread::ThreadFunc  ThreadFunc;
    ThreadFunc m_func;
    std::string m_name;
    pid_t* m_tid;
    CountDownLatch* m_latch;

    ThreadData(ThreadFunc func,
               const std::string& name,
               pid_t* tid,
               CountDownLatch* latch)
      : m_func(std::move(func)),
        m_name(name),
        m_tid(tid),
        m_latch(latch)
    { }

    void runInThread()
    {
     *m_tid = base::CurrentThread::tid();
      m_tid = NULL;
      m_latch->countDown();
      m_latch = NULL;

      base::CurrentThread::t_threadName = m_name.empty() ? "baseLibraryThread" : m_name.c_str();
      ::prctl(PR_SET_NAME, base::CurrentThread::t_threadName); 
      try
      {
        m_func();
        base::CurrentThread::t_threadName = "finished";
      }
      catch (const Exception& ex)
      {
        base::CurrentThread::t_threadName = "crashed";
        fprintf(stderr, "exception caught in Thread %s\n", m_name.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
        abort();
      }
      catch (const std::exception& ex)
      {
        base::CurrentThread::t_threadName = "crashed";
        fprintf(stderr, "exception caught in Thread %s\n", m_name.c_str());
        fprintf(stderr, "reason: %s\n", ex.what());
        abort();
      }
      catch(...)
      {
        base::CurrentThread::t_threadName = "crashed";
        fprintf(stderr, "unknown exception caught in Thread %s\n", m_name.c_str());
        throw; // rethrow
      }
      
    }
  };  // struct  ThreadData

void* startThread(void* obj)
{
  ThreadData*  data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}   

} // namespace detail
 
 AtomicInt32 Thread::m_numCreated;

 Thread::Thread(ThreadFunc func, const std::string& n)
   : m_started(false),
     m_joined(false),
     m_pthreadId(0),
     m_tid(0),
     m_func(std::move(func)),
     m_name(n),
     m_latch(1)
  {
        setDefaultName();
  }

Thread::~Thread()
{
  if(m_started && !m_joined)
  {
    pthread_detach(m_pthreadId);
  }
}

  void Thread::setDefaultName()
  {
    int num = m_numCreated.incrementAndGet(); 
    if (m_name.empty())  
    {
       char buf[32];
       snprintf(buf, sizeof buf, "Thread%d", num);   
       m_name = buf; 
    }
  }  

  void Thread::start()
  {
    assert(!m_started);
    m_started = true;
    detail::ThreadData* data = new detail::ThreadData(m_func, m_name, &m_tid, &m_latch);
    if (pthread_create(&m_pthreadId, NULL, &detail::startThread, data)) 
    {
      m_started = false;
      delete data;
      std::cout << "LOG:Failed in pthread_create" << std::endl;

    }
    else
    {
      m_latch.wait();
      assert(m_tid > 0);
    }
  }

  int Thread::join()
  {
    assert(m_started);
    assert(!m_joined);
    m_joined = true;
    return pthread_join(m_pthreadId, NULL);
  }


}  // namespace base

