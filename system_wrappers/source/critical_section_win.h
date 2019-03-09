#ifndef GN_SYSTEM_WRAPPERS_SOURCE_CRITICAL_SECTION_WIN_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_CRITICAL_SECTION_WIN_H_

#include <windows.h>
#include "system_wrappers/interface/critical_section_wrapper.h"
#include "typedefs.h"

namespace gn
{
	class CriticalSectionWindows : public CriticalSectionWrapper
	{
	public:
		CriticalSectionWindows();

		virtual ~CriticalSectionWindows();

		virtual void Enter();
		virtual void Leave();

	private:
		CRITICAL_SECTION crit;

		friend class ConditionVariableEventWin;
		friend class ConditionVariableNativeWin;
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_SOURCE_CRITICAL_SECTION_WIN_H_
