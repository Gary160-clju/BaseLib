//2021/07/14
#ifndef _BASE_MUTEX_H
#define _BASE_MUTEX_H
#include "base/CurrentThread.h"
#include "base/noncopyable.h"
#include <assert.h>
#include <pthread.h>


#ifdef CHECK_PTHREAD_RETURN_VALUE     ///////////////��û�ж���CHECK_PTHREAD_RETURN_VALUE��ʱ�򣬱༭�����ܵ����ε���Ӧ��Ч����䡣

#ifdef NDEBUG
__BEGIN_DECLS
extern void __assert_perror_fail(int errnum,
	const char* file,
	unsigned int line,
	const char* function)
	noexcept __attribute__((__noreturn__));
__END_DECLS
#endif

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else 
#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

#endif // CHECK_PTHREAD_RETURN_VALUE



namespace liuSown
{

class MutexLock : noncopyable      
{
public:
	MutexLock()
		: holder_(0)
	{
		MCHECK(pthread_mutex_init(&mutex_, NULL));//��ʼ��������������飨MCHECK���Ƿ��ʼ���ɹ���
	}

	~MutexLock()
	{
		assert(holder_ == 0);  //�����߲�Ϊ0�����������������
		MCHECK(pthread_mutex_destroy(&mutex_));
	}

	// must be called when locked, i.e. for assertion
	bool isLockedByThisThread() const   //�жϻ������Ƿ񱻵�ǰ�߳���ʹ��
	{
		return holder_ == CurrentThread::tid();      
	}
	void assertLocked()
	{
		assert(isLockedByThisThread());
	}
	void lock()
	{
		pthread_mutex_lock(&mutex_); //����MutexLockGuard���ã��Ͻ��û��������
		assignHolder();
	}
	void unlock()   //����MutexLockGuard���ã��Ͻ��û��������
	{
		unassignHolder();
		pthread_mutex_unlock(&mutex_);
	}

	pthread_mutex_t*  getPthreadMutex()  //���� Condition ���ã��Ͻ��û��������
	{
		return   &mutex_;
	}


private:
	friend class Condition;  //�������������������֮�����ʹ��



	class UnassignGuard : noncopyable 
	{
	public:
		explicit UnassignGuard(MutexLock& owner)
			: owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard()   //��ʾ�����ͷ�
		{
			owner_.assignHolder();
		}

	private:
		MutexLock& owner_;
	};//UnassignGuard���������������ʹ�õġ�



	void unassignHolder()
	{
		holder_ = 0;
	}

	void assignHolder()
	{
		holder_ = CurrentThread::tid();
	}

	pthread_mutex_t mutex_;
	pid_t  holder_;  

};



class  MutexLockGuard : noncopyable 
{
public:
	explicit MutexLockGuard(MutexLock& mutex)
		: mutex_(mutex)
	{
		mutex_.lock();
	}

	~MutexLockGuard()
	{
		mutex_.unlock();
	}

private:
	MutexLock& mutex_;

};
}  // namespace liuSown

#endif  // _BASE_MUTEX_H