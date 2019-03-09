#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_CONDITION_VARIABLE_WRAPPER_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_CONDITION_VARIABLE_WRAPPER_H_

#include "typedefs.h"

namespace gn
{

	class CriticalSectionWrapper;

	class ConditionVariableWrapper
	{
	public:
		// Factory method, constructor disabled.
		static ConditionVariableWrapper* CreateConditionVariable();

		virtual ~ConditionVariableWrapper() {}

		// Calling thread will atomically release crit_sect and wait until next
		// some other thread calls Wake() or WakeAll().
		virtual void SleepCS(CriticalSectionWrapper& crit_sect) = 0;

		// Same as above but with a timeout.
		virtual bool SleepCS(CriticalSectionWrapper& crit_sect,
			unsigned long max_time_in_ms) = 0;

		// Wakes one thread calling SleepCS().
		virtual void Wake() = 0;

		// Wakes all threads calling SleepCS().
		virtual void WakeAll() = 0;
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_CONDITION_VARIABLE_WRAPPER_H_
