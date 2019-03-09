#ifndef _ASYN_EVENT_TEST_
#define _ASYN_EVENT_TEST_

class AsynEvent
{
public:
    static AsynEvent* createAsynEvent();
    virtual ~AsynEvent() {}

public:
    virtual void test() {}
};

#endif // _ASYN_EVENT_TEST_
