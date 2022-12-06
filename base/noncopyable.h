#ifndef _BASE_NONCOPYABLE_H
#define _BASE_NONCOPYABLE_H

namespace base
{

	class noncopyable//禁止拷贝操作,实现不可拷贝类.让自己的类继承自noncopyable 即可
	{
	public:
		noncopyable(const noncopyable&) = delete;   
		void operator=(const noncopyable&) = delete;

	protected:
		noncopyable() = default;
		~noncopyable() = default;
	};

}  // namespace base

#endif  // _BASE_NONCOPYABLE_H
