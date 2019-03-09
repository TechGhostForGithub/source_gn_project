#if defined(CCORE_WIN) || defined(_MSC_VER)
#include "system_wrappers/source/timer_win.h"
#elif defined(CCORE_MAC) || defined(CCORE_IOS)
#include "system_wrappers/source/timer_oc.h"
#elif defined(CCORE_ANDROID) || defined(CCORE_POSIX)
#include "system_wrappers/source/timer_posix.h"
#endif

namespace gn
{
	TimerWrapper* TimerWrapper::CreateTimer()
	{
#if defined(CCORE_WIN) || defined(_MSC_VER)
		return new TimerWin();
#elif defined(CCORE_MAC) || defined(CCORE_IOS)
        return new TimerOc();
#elif defined(CCORE_ANDROID) || defined(CCORE_POSIX)
        return new TimerPosix();
#else
		return NULL;
#endif
	}

}  // namespace gn
