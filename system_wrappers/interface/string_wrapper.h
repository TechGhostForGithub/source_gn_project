// System independant wrapper for polling elapsed time in ms and us.
// The implementation works in the tick domain which can be mapped over to the
// time domain.
#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_STRING_WRAPPER_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_STRING_WRAPPER_H_

#include <string>

#include "typedefs.h"

namespace gn {
    namespace String {
        std::string StringFormatA(const char *fmt, ...);

        std::wstring StringFormatW(const wchar_t *fmt, ...);

        void StringReplace(std::string &strBase, std::string strSrc, std::string strDes);
    }
}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_STRING_WRAPPER_H_
