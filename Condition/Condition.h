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
		MCHECK(pthread_cond_init(&pcond_, NULL));   ////MCHECK��muduo�ṩ�ĺ궨����麯��  
	}
	 

	~Condition()  
	{
		MCHECK(pthread_cond_destroy(&pcond_));
	}

	void wait()  //ʹ��֮ǰ��Ҫռ�л�����
	{
		MutexLock::UnassignGuard ug(mutex_);    //Ŀǰ�����������ڹ�����Mutex������ǰ�߳�������
		pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
	}

	// returns true if time out, false otherwise.
	bool waitForSeconds(double seconds);

	void notify() //֪ͨ��ͨ��
	{
		pthread_cond_signal(&pcond_);   //֪ͨһ���߳�
	}

	void notifyAll()  //֪ͨ���е��߳�
	{
		pthread_cond_broadcast(&pcond_);
	}
private:
	MutexLock& mutex_;
	pthread_cond_t pcond_;
};

}  // namespace liuSown

#endif  //_BASE_CONDITION_H



