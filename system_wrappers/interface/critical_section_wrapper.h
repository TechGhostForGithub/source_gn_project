#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_CRITICAL_SECTION_WRAPPER_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_CRITICAL_SECTION_WRAPPER_H_

// If the critical section is heavily contended it may be beneficial to use
// read/write locks instead.

#include "common_types.h"

namespace gn
{
	class CriticalSectionWrapper
	{
	public:
		// Factory method, constructor disabled
		static CriticalSectionWrapper* CreateCriticalSection();

		virtual ~CriticalSectionWrapper() {}

		// Tries to grab lock, beginning of a critical section. Will wait for the
		// lock to become available if the grab failed.
		virtual void Enter() = 0;

		// Returns a grabbed lock, end of critical section.
		virtual void Leave() = 0;
	};

	// RAII extension of the critical section. Prevents Enter/Leave mismatches and
	// provides more compact critical section syntax.
	class CriticalSectionScoped {
	public:
		explicit CriticalSectionScoped(CriticalSectionWrapper* critsec)
			: ptr_crit_sec_(critsec)
		{
			ptr_crit_sec_->Enter();
		}

		~CriticalSectionScoped()
		{
			if (ptr_crit_sec_)
			{
				Leave();
			}
		}

	private:
		void Leave()
		{
			ptr_crit_sec_->Leave();
			ptr_crit_sec_ = 0;
		}

		CriticalSectionWrapper* ptr_crit_sec_;
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_CRITICAL_SECTION_WRAPPER_H_
