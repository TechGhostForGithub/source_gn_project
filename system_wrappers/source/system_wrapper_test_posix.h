#ifndef _SYSTEM_WRAPPER_TEST_POSIX_
#define _SYSTEM_WRAPPER_TEST_POSIX_
#include "system_wrappers/interface/system_wrapper_test.h"

class SystemWrapperTestPosix : public SystemWrapperTest
{
public:
    SystemWrapperTestPosix();
    virtual ~SystemWrapperTestPosix();

public:
    virtual void test();
};

#endif // _SYSTEM_WRAPPER_TEST_POSIX_