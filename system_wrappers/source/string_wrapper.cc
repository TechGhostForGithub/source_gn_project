
#include "system_wrappers/interface/string_wrapper.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <vector>
#include <string.h>

namespace gn {
    namespace String {
        std::string StringFormatA(const char *fmt, ...) 
        { 
            std::string strResult="";
            if (NULL != fmt)
            {
                char buf[1024] = {0};
                va_list args;
                va_start(args, fmt);
#ifdef _WIN32
                _vsnprintf(buf, sizeof(buf) - 1, fmt, args);
#else
                vsnprintf(buf, sizeof(buf) - 1, fmt, args);
#endif                 
                va_end(args);
                strResult = buf; 
            }
            return strResult; 
        }

        std::wstring StringFormatW(const wchar_t *fmt, ...) 
        { 
            std::wstring strResult=L"";
            if (NULL != fmt)
            {
                wchar_t buf[1024] = {0};
                va_list args;
                va_start(args, fmt);
#ifdef _WIN32
                _vsnwprintf(buf, sizeof(buf) - 1, fmt, args);
#else
                vswprintf(buf, sizeof(buf) - 1, fmt, args);
#endif                
                va_end(args);
                strResult = buf;
            }
            return strResult; 
        }

        void StringReplace(std::string &strBase, std::string strSrc, std::string strDes)  
        {  
            std::string::size_type pos = 0;  
            std::string::size_type srcLen = strSrc.size();  
            std::string::size_type desLen = strDes.size();  
            pos = strBase.find(strSrc, pos);   
            while ((pos != std::string::npos))  
            {  
                strBase.replace(pos, srcLen, strDes);  
                pos = strBase.find(strSrc, (pos+desLen));  
            }  
        }
    }
}  // namespace gn
