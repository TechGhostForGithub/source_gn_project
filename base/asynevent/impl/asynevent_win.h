#ifndef _ASYN_EVENT_WIN_
#define _ASYN_EVENT_WIN_
#include "base/asynevent/asynevent_test.h"

class AsynEventWin: public AsynEvent
{
public:
    AsynEventWin() {}
    virtual ~AsynEventWin() {}

public:
    virtual void test();
};

#endif // _ASYN_EVENT_WIN_
