#include "system_wrappers/source/critical_section_win.h"

namespace gn
{

	CriticalSectionWindows::CriticalSectionWindows()
	{
		InitializeCriticalSection(&crit);
	}

	CriticalSectionWindows::~CriticalSectionWindows()
	{
		DeleteCriticalSection(&crit);
	}

	void CriticalSectionWindows::Enter()
	{
		EnterCriticalSection(&crit);
	}

	void CriticalSectionWindows::Leave()
	{
		LeaveCriticalSection(&crit);
	}

}  // namespace gn
