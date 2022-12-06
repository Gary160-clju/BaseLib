#include "CurrentThread.h"

namespace base
{

namespace CurrentThread
{
     const char* t_threadName = "unknown";  
    
     std::string stackTrace(bool demangle)
     {
        std::string stack;
        const int max_frames = 200;
        void* frame[max_frames];
        int nptrs = ::backtrace(frame, max_frames); 
        char** strings = ::backtrace_symbols(frame, nptrs);  
        if (strings)
        {
           size_t len = 256;
           char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
           for (int i = 1; i < nptrs; ++i)  
           {
             if (demangle)
             {
               char* left_par = nullptr;
               char* plus = nullptr;
               for (char* p = strings[i]; *p; ++p)
               {
                 if (*p == '(')
                 left_par = p;
                 else if (*p == '+')
                 plus = p;
                }

              if (left_par && plus)
              {
                 *plus = '\0';
                  int status = 0;
                  char* ret = abi::__cxa_demangle(left_par+1, demangled, &len, &status);  //将C++内部改编的函数名还原     
                  *plus = '+';
                  if (status == 0)
                  {
                     demangled = ret;  
                     stack.append(strings[i], left_par+1);
                     stack.append(demangled);
                     stack.append(plus);
                     stack.push_back('\n');
                     continue;
                  }
               }
             }
                
                stack.append(strings[i]);
                stack.push_back('\n');
            }
            free(demangled);
            free(strings);
        }
      return stack;
    }

}   //  namespace CurrentThread

pid_t gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

}  //  namespace base