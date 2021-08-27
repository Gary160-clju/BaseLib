#ifndef _BASE_COUNTDOWNLATCH_H
#define _BASE_COUNTDOWNLATCH_H

#include "base/Condition.h"
#include "base/Mutex.h"

namespace liuSown
{

   class CountDownLatch : noncopyable 
	{
	public:

		explicit CountDownLatch(int count);  //倒数几次

		void wait();   //等待计数值变为 0

		void countDown();   //计数减一

		int getCount() const;

	private:
		mutable MutexLock mutex_;   //顺序很重要，先 mutex 后 condition
		Condition condition_;//GUARDED_BY(mutex_);
		int count_;// GUARDED_BY(mutex_);
	};
}  // namespace liuSown
#endif  // _BASE_COUNTDOWNLATCH_H

