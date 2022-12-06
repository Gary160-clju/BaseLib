#include "base/Exception.h"
#include "base/CurrentThread.h"

namespace base
{

Exception::Exception(std::string msg)
  : m_message(std::move(msg)),
    m_stack(CurrentThread::stackTrace(false))
{
}

}  // namespace base
