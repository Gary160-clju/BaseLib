#ifndef _BASE_CURRENTTHREAD_H
#define _BASE_CURRENTTHREAD_H

#include <unistd.h>
#include <sys/syscall.h>

namespace base
{
namespace CurrentThread
{ 
    extern  const char* t_threadName;
    pid_t gettid();

    inline int tid()
    {
       int systid = 0;
       systid = gettid();
       return systid;
    }

}  //namespace CurrentThread
}  //namespace base

#endif  // _BASE_CURRENTTHREAD_H