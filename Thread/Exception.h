#ifndef _BASE_EXCEPTION_H
#define _BASE_EXCEPTION_H

#include "base/Types.h"
#include <exception>

namespace liuSown
{
    //继承自标准模板库exception，主要功能为记录异常发生时的堆栈信息。
    class Exception : public std::exception
    {
    public:
        Exception(string what);
        ~Exception() noexcept override = default;

        // default copy-ctor and operator= are okay.

        const char* what() const noexcept override   //返回message
        {
            return message_.c_str();
        }

        const char* stackTrace() const noexcept    //返回stack
        {
            return stack_.c_str();
        }

    private:
        string message_;  //存储异常信息字符串
        string stack_;     //存储异常发生时栈信息
    };

}  // namespace liuSown

#endif  // _BASE_EXCEPTION_H
