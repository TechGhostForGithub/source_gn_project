#ifndef _SOCKET_TEST_
#define _SOCKET_TEST_
#include "base/asynevent/asynevent_test.h"

class SocketTest
{
public:
    SocketTest();
    ~SocketTest();

public:
    void test();

private:
    AsynEvent* m_asynEvent;
};

#endif // _SOCKET_TEST_