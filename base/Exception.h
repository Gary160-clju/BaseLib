#ifndef _BASE_EXCEPTION_H
#define _BASE_EXCEPTION_H

#include "base/Types.h"
#include <exception>

namespace base
{
class Exception : public std::exception
{
 public:
  Exception(string what);
  ~Exception() noexcept override = default;


  const char* what() const noexcept override   
  {
    return m_message.c_str();
  }

  const char* stackTrace() const noexcept   
  {
    return m_stack.c_str();
  }

 private:
  std::string m_message;  
  std::string m_stack;    
};

}  // namespace base

#endif  //  _BASE_EXCEPTION_H

