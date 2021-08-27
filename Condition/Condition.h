#ifndef _BASE_CONDITION_H
#define _BASE_CONDITION_H
#include "base/Mutex.h"
#include <pthread.h>
namespace liuSown
{
class Condition : noncopyable
{   
public:
	explicit Condition(MutexLock& mutex)
		: mutex_(mutex)
	{
		MCHECK(pthread_cond_init(&pcond_, NULL));   ////MCHECK是muduo提供的宏定义检验函数  
	}
	 

	~Condition()  
	{
		MCHECK(pthread_cond_destroy(&pcond_));
	}

	void wait()  //使用之前需要占有互斥量
	{
		MutexLock::UnassignGuard ug(mutex_);    //目前条件变量所在关联的Mutex不被当前线程所持有
		pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
	}

	// returns true if time out, false otherwise.
	bool waitForSeconds(double seconds);

	void notify() //通知，通报
	{
		pthread_cond_signal(&pcond_);   //通知一条线程
	}

	void notifyAll()  //通知所有的线程
	{
		pthread_cond_broadcast(&pcond_);
	}
private:
	MutexLock& mutex_;
	pthread_cond_t pcond_;
};

}  // namespace liuSown

#endif  //_BASE_CONDITION_H



