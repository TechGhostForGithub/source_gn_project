#include "base/socket/socket_test.h"
#include "common_types.h"

SocketTest::SocketTest()
    : m_asynEvent(NULL)
{
    m_asynEvent = AsynEvent::createAsynEvent();
}

SocketTest::~SocketTest()
{
    SAFE_DELETE_PTR(m_asynEvent);
}

void SocketTest::test()
{
    // do something
}