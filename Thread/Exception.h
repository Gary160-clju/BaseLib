#ifndef _BASE_EXCEPTION_H
#define _BASE_EXCEPTION_H

#include "base/Types.h"
#include <exception>

namespace liuSown
{
    //�̳��Ա�׼ģ���exception����Ҫ����Ϊ��¼�쳣����ʱ�Ķ�ջ��Ϣ��
    class Exception : public std::exception
    {
    public:
        Exception(string what);
        ~Exception() noexcept override = default;

        // default copy-ctor and operator= are okay.

        const char* what() const noexcept override   //����message
        {
            return message_.c_str();
        }

        const char* stackTrace() const noexcept    //����stack
        {
            return stack_.c_str();
        }

    private:
        string message_;  //�洢�쳣��Ϣ�ַ���
        string stack_;     //�洢�쳣����ʱջ��Ϣ
    };

}  // namespace liuSown

#endif  // _BASE_EXCEPTION_H
