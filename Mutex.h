//2021/07/14
#ifndef _BASE_MUTEX_H
#define _BASE_MUTEX_H
#include "base/CurrentThread.h"
#include "base/noncopyable.h"
#include <assert.h>
#include <pthread.h>


#ifdef CHECK_PTHREAD_RETURN_VALUE     ///////////////当没有定义CHECK_PTHREAD_RETURN_VALUE的时候，编辑器智能地屏蔽掉相应无效的语句。

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
		MCHECK(pthread_mutex_init(&mutex_, NULL));//初始化互斥锁，并检查（MCHECK）是否初始化成功。
	}

	~MutexLock()
	{
		assert(holder_ == 0);  //持有者不为0则出错，不能销毁锁。
		MCHECK(pthread_mutex_destroy(&mutex_));
	}

	// must be called when locked, i.e. for assertion
	bool isLockedByThisThread() const   //判断互斥量是否被当前线程所使用
	{
		return holder_ == CurrentThread::tid();      
	}
	void assertLocked()
	{
		assert(isLockedByThisThread());
	}
	void lock()
	{
		pthread_mutex_lock(&mutex_); //仅供MutexLockGuard调用，严禁用户代码调用
		assignHolder();
	}
	void unlock()   //仅供MutexLockGuard调用，严禁用户代码调用
	{
		unassignHolder();
		pthread_mutex_unlock(&mutex_);
	}

	pthread_mutex_t*  getPthreadMutex()  //仅供 Condition 调用，严禁用户代码调用
	{
		return   &mutex_;
	}


private:
	friend class Condition;  //条件变量必须持有了锁之后才能使用



	class UnassignGuard : noncopyable 
	{
	public:
		explicit UnassignGuard(MutexLock& owner)
			: owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard()   //表示锁被释放
		{
			owner_.assignHolder();
		}

	private:
		MutexLock& owner_;
	};//UnassignGuard是配合条件变量来使用的。



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