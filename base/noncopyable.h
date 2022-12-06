#ifndef _BASE_NONCOPYABLE_H
#define _BASE_NONCOPYABLE_H

namespace base
{

	class noncopyable//��ֹ��������,ʵ�ֲ��ɿ�����.���Լ�����̳���noncopyable ����
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
