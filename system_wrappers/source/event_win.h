#ifndef GN_SYSTEM_WRAPPERS_SOURCE_EVENT_WIN_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_EVENT_WIN_H_

#include <windows.h>

#include "system_wrappers/interface/event_wrapper.h"

#include "typedefs.h"

namespace gn
{

	class EventWindows : public EventWrapper
	{
	public:
		EventWindows();
		virtual ~EventWindows();

		virtual EventTypeWrapper Wait(unsigned long max_time);
		virtual bool Set();
		virtual bool Reset();

		virtual bool StartTimer(bool periodic, unsigned long time);
		virtual bool StopTimer();

	private:
		HANDLE  event_;
		uint32_t timerID_;
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_SOURCE_EVENT_WIN_H_
