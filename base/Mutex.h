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
         pid_t holder_;
         friend class Condition;
         class UnassignGuard : noncopyable
         {
            public:
             UnassignGuard(MutexLock& owner)
              : m_owner(owner)
              {
               m_owner.unassignHolder();
              }

              ~UnassignGuard()
              {
               m_owner.assignHolder();
              }

            private:
             MutexLock& m_owner;
         };

         void unassignHolder()
         {
            holder_ = 0;
         }

         void assignHolder()
         {
            holder_ = CurrentThread::tid(); 
         }

        public:
         MutexLock()
           : holder_(0)
         {
            int status;
            status = pthread_mutex_init(&m_mutex, NULL);
            if(status != 0)
            {
                abort();  
            }
               
         }

         ~MutexLock()
         {
            assert(holder_ == 0);
            int status;
            status = pthread_mutex_destroy(&m_mutex);
            if(status != 0)
            {
               abort();
            }
         }

         //当前线程是否拥有该锁
         bool isLockedByThisThread() const
         {
            return holder_ == CurrentThread::tid();
         }

         //断言当前线程锁拥有该锁
         void assertLocked() const 
         {
            assert(isLockedByThisThread());
         }

         void lock()
         {
            int status;
            status = pthread_mutex_lock(&m_mutex);
            if(status != 0)
            {
               abort();
            }
            assignHolder();
         }

         void unlock()
         {
            unassignHolder();
            int status;
            status = pthread_mutex_unlock(&m_mutex);
            if(status != 0)
            {
               abort();
            }
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

