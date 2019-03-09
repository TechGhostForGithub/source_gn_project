#include "base/http/http_test.h"
#include "common_types.h"

HttpTest::HttpTest()
    : m_socket(NULL)
{
    m_socket = new SocketTest();
}

HttpTest::~HttpTest()
{
    SAFE_DELETE_PTR(m_socket);
}

void HttpTest::test()
{
    // do something
}