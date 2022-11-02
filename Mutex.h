#ifndef _BASE_MUTEX_H
#define _BASE_MUTEX_H

#include "base/CurrentThread.h"
#include "base/noncopyable.h"
#include <assert.h>
#include <pthread.h>


namespace base
{

    class MutexLock : noncopyable
    {
        private:
         pthread_mutex_t m_mutex;

        public:
         MutexLock()
         {
            pthread_mutex_init(&m_mutex, NULL);
         }

         ~MutexLock()
         {
            pthread_mutex_destroy(&m_mutex);
         }

         void lock()
         {
            pthread_mutex_lock(&m_mutex);
         }

         void unlock()
         {
            pthread_mutex_unlock(&m_mutex);
         }

         pthread_mutex_t* getPthreadMutex()
         {
            return &m_mutex;
         }
    }; // class MutexLock

    class  MutexLockGuard : noncopyable
    {
        private:
         MutexLock& m_mutex;
         public:
          MutexLockGuard(MutexLock& mutex)
            : m_mutex(mutex)
          {
            m_mutex.lock();
          }

          ~MutexLockGuard()
          {
            m_mutex.unlock();
          }
    }; // class MutexLockGuard
    
}  // namespace base

#endif // _BASE_MUTEX_H 

