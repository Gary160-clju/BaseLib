#ifndef _BASE_THREAD_H
#define _BASE_THREAD_H

#include "base/Atomic.h"
#include "base/CountDownLatch.h"
#include "base/Types.h"

#include <functional>
#include <memory>
#include <pthread.h>

namespace base
{
    class Thread : noncopyable
    {
        
        public:
         typedef std::function<void ()> ThreadFunc;
         Thread(ThreadFunc, const std::string& name = std::string());
         ~Thread();

         void start();
         int join();

        private:
         void setDefaultName();

         bool m_started;
         bool m_joined;
         pthread_t m_pthreadId;

         pid_t m_tid;
         ThreadFunc  m_func;
         std::string m_name;
         CountDownLatch m_latch;
         
         static AtomicInt32 m_numCreated; 
         
    };  //class Thread   

}   // namespace base
#endif  // _BASE_THREAD_H