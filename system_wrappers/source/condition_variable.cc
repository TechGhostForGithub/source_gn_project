#include "system_wrappers/interface/condition_variable_wrapper.h"

#if defined(_WIN32)
#include <windows.h>
#include "system_wrappers/source/condition_variable_event_win.h"
#include "system_wrappers/source/condition_variable_native_win.h"
#elif defined(CCORE_LINUX) || defined(CCORE_MAC)
#include <pthread.h>
#include "system_wrappers/source/condition_variable_posix.h"
#endif

namespace gn
{

	ConditionVariableWrapper* ConditionVariableWrapper::CreateConditionVariable()
	{
#if defined(_WIN32)
		// Try to create native condition variable implementation.
		ConditionVariableWrapper* ret_val = ConditionVariableNativeWin::Create();
		if (!ret_val)
		{
			// Native condition variable implementation does not exist. Create generic
			// condition variable based on events.
			ret_val = new ConditionVariableEventWin();
		}
		return ret_val;
#elif defined(CCRORE_LINUX) || defined(CCORE_MAC) || defined(CCORE_IOS)
		return ConditionVariablePosix::Create();
#else
		return NULL;
#endif
	}

}  // namespace gn
