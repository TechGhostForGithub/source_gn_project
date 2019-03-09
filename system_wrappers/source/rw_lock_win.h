#ifndef GN_SYSTEM_WRAPPERS_SOURCE_RW_LOCK_WIN_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_RW_LOCK_WIN_H_

#include "system_wrappers/interface/rw_lock_wrapper.h"

#include <Windows.h>

namespace gn
{

	class RWLockWin : public RWLockWrapper
	{
	public:
		static RWLockWin* Create();
		~RWLockWin() {}

		virtual void AcquireLockExclusive();
		virtual void ReleaseLockExclusive();

		virtual void AcquireLockShared();
		virtual void ReleaseLockShared();

	private:
		RWLockWin();
		static bool LoadModule();

		SRWLOCK lock_;
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_SOURCE_RW_LOCK_WIN_H_
