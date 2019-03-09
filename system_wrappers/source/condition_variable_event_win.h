#ifndef GN_SYSTEM_WRAPPERS_SOURCE_CONDITION_VARIABLE_EVENT_WIN_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_CONDITION_VARIABLE_EVENT_WIN_H_

#include <windows.h>

#include "system_wrappers/interface/condition_variable_wrapper.h"

namespace gn
{

	class ConditionVariableEventWin : public ConditionVariableWrapper
	{
	public:
		ConditionVariableEventWin();
		virtual ~ConditionVariableEventWin();

		void SleepCS(CriticalSectionWrapper& crit_sect);
		bool SleepCS(CriticalSectionWrapper& crit_sect, unsigned long max_time_inMS);
		void Wake();
		void WakeAll();

	private:
		enum EventWakeUpType
		{
			WAKEALL_0   = 0,
			WAKEALL_1   = 1,
			WAKE        = 2,
			EVENT_COUNT = 3
		};

		unsigned int     num_waiters_[2];
		EventWakeUpType  eventID_;
		CRITICAL_SECTION num_waiters_crit_sect_;
		HANDLE           events_[EVENT_COUNT];
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_SOURCE_CONDITION_VARIABLE_EVENT_WIN_H_
