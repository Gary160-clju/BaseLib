#ifndef _BASE_NONCOPYABLE_H
#define _BASE_NONCOPYABLE_H

namespace liuSown
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

}  // namespace liuSown

#endif  // _BASE_NONCOPYABLE_H
