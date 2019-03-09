#ifndef _SYSTEM_WRAPPER_TEST_OC_
#define _SYSTEM_WRAPPER_TEST_OC_
#include "system_wrappers/interface/system_wrapper_test.h"

class SystemWrapperTestOC : public SystemWrapperTest
{
public:
    SystemWrapperTestOC();
    virtual ~SystemWrapperTestOC();

public:
    virtual void test();
};

#endif // _SYSTEM_WRAPPER_TEST_OC_