#ifndef _BASE_NONCOPYABLE_H
#define _BASE_NONCOPYABLE_H

namespace liuSown
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

}  // namespace liuSown

#endif  // _BASE_NONCOPYABLE_H
