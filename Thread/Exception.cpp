#include "base/Exception.h"
#include "base/CurrentThread.h"

namespace liuSown
{

    Exception::Exception(string msg)
        : message_(std::move(msg)),
        stack_(CurrentThread::stackTrace(/*demangle=*/false))
    {
    }

}  // namespace liuSown
