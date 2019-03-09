#include "system_wrappers/interface/event_wrapper.h"

#if defined(_WIN32)
#include <windows.h>
#include "system_wrappers/source/event_win.h"
#elif defined(CCORE_MAC) && !defined(CCORE_IOS)
#include <ApplicationServices/ApplicationServices.h>
#include <pthread.h>
#include "system_wrappers/source/event_posix.h"
#else
#include <pthread.h>
#include "system_wrappers/source/event_posix.h"
#endif

namespace gn
{
	EventWrapper* EventWrapper::Create()
	{
#if defined(_WIN32)
		return new EventWindows();
#else
		return EventPosix::Create();
#endif
	}
}  // namespace gn
