#include "base/asynevent/asynevent_test.h"
#include "common_types.h"
#if defined(CCORE_WIN) ||defined(_MSC_VER)
#include "base/asynevent/impl/asynevent_win.h"
#elif defined(CCORE_MAC) || defined(CCORE_IOS) || defined(CCORE_ANDROID) || defined(CCORE_POSIX)
#include "base/asynevent/impl/asynevent_posix.h"
#endif

AsynEvent* AsynEvent::createAsynEvent()
{
#if defined(CCORE_WIN) ||defined(_MSC_VER)
    return new AsynEventWin();
#elif defined(CCORE_MAC) || defined(CCORE_IOS) || defined(CCORE_ANDROID) || defined(CCORE_POSIX)
    return new AsynEventPosix();
#else
    return NULL;
#endif
}