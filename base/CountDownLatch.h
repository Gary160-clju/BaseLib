#ifndef _BASE_COUNTDOWNLATCH_H
#define _BASE_COUNTDOWNLATCH_H

#include "base/Condition.h"
#include "base/Mutex.h"

namespace base
{

class CountDownLatch : noncopyable
{
 public:

  CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock m_mutex;
  Condition m_condition;   //GUARDED_BY��mutex_����������clang�������̰߳�ȫ���Ĳ���
  int m_count;
};

}  // namespace base
#endif  // _BASE_COUNTDOWNLATCH_H
