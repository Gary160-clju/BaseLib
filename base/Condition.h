#ifndef _BASE_CONDITION_H
#define _BASE_CONDITION_H

#include "base/Mutex.h"

namespace base
{
    class Condition : noncopyable
    {
        private:
         MutexLock& m_mutex;
         pthread_cond_t m_pcond;
         
        public:
         Condition(MutexLock& mutex)
          : m_mutex(mutex)
         {
            int status;
            status = pthread_cond_init(&m_pcond, NULL);
            if(status != 0)
            {
               abort();
            }
         }

         ~Condition()
         {
            int status;
            status = pthread_cond_destroy(&m_pcond);
            if(status != 0)
            {
               abort();
            }
         }

         void wait()
         {
            MutexLock::UnassignGuard ug(m_mutex);
            int status;
            status = pthread_cond_wait(&m_pcond, m_mutex.getPthreadMutex());
            if(status != 0)
            {
               abort();
            }
         }

         void notify()
         {
           int status;
           status = pthread_cond_signal(&m_pcond);
           if(status != 0)
           {
               abort();
           }
         }

         void notifyAll()
         {
           int status;
           status = pthread_cond_broadcast(&m_pcond);
           if(status != 0)
           {
             abort();
           }
         }

      };

} // namespace base
#endif  // _BASE_CONDITION_H