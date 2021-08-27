#ifndef _BASE_COUNTDOWNLATCH_H
#define _BASE_COUNTDOWNLATCH_H

#include "base/Condition.h"
#include "base/Mutex.h"

namespace liuSown
{

   class CountDownLatch : noncopyable 
	{
	public:

		explicit CountDownLatch(int count);  //��������

		void wait();   //�ȴ�����ֵ��Ϊ 0

		void countDown();   //������һ

		int getCount() const;

	private:
		mutable MutexLock mutex_;   //˳�����Ҫ���� mutex �� condition
		Condition condition_;//GUARDED_BY(mutex_);
		int count_;// GUARDED_BY(mutex_);
	};
}  // namespace liuSown
#endif  // _BASE_COUNTDOWNLATCH_H

