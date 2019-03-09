#ifndef _ASYN_EVENT_POSIX_
#define _ASYN_EVENT_POSIX_
#include "base/asynevent/asynevent_test.h"

class AsynEventPosix: public AsynEvent
{
public:
    AsynEventPosix() {}
    virtual ~AsynEventPosix() {}

public:
    virtual void test();
};

#endif // _ASYN_EVENT_POSIX_
