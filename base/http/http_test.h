#ifndef _HTTP_TEST_
#define _HTTP_TEST_
#include "base/socket/socket_test.h"

class HttpTest
{
public:
    HttpTest();
    ~HttpTest();

public:
    void test();

private:
    SocketTest* m_socket;
};

#endif // _HTTP_TEST_