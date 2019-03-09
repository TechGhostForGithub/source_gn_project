﻿#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_STATIC_INSTANCE_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_STATIC_INSTANCE_H_

#include <assert.h>

#include "system_wrappers/interface/critical_section_wrapper.h"
#ifdef _WIN32
#include "system_wrappers/interface/fix_interlocked_exchange_pointer_win.h"
#endif

namespace gn
{
	enum CountOperation
	{
		kRelease,
		kAddRef,
		kAddRefNoCreate
	};
	enum CreateOperation
	{
		kInstanceExists,
		kCreate,
		kDestroy
	};

	template <class T>
	// Construct On First Use idiom. Avoids
	// "static initialization order fiasco".
	static T* GetStaticInstance(CountOperation count_operation)
	{
		// TODO (hellner): use atomic wrapper instead.
		static volatile long instance_count = 0;
		static T* volatile instance = NULL;
		CreateOperation state = kInstanceExists;
#ifndef _WIN32
		// This memory is staticly allocated once. The application does not try to
		// free this memory. This approach is taken to avoid issues with
		// destruction order for statically allocated memory. The memory will be
		// reclaimed by the OS and memory leak tools will not recognize memory
		// reachable from statics leaked so no noise is added by doing this.
		static CriticalSectionWrapper* crit_sect(
			CriticalSectionWrapper::CreateCriticalSection());
		CriticalSectionScoped lock(crit_sect);

		if (count_operation ==
			kAddRefNoCreate && instance_count == 0)
		{
			return NULL;
		}
		if (count_operation ==
			kAddRef ||
			count_operation == kAddRefNoCreate)
		{
			instance_count++;
			if (instance_count == 1)
			{
				state = kCreate;
			}
		}
		else
		{
			instance_count--;
			if (instance_count == 0)
			{
				state = kDestroy;
			}
		}
		if (state == kCreate)
		{
			instance = T::CreateInstance();
		}
		else if (state == kDestroy)
		{
			T* old_instance = instance;
			instance = NULL;
			// The state will not change past this point. Release the critical
			// section while deleting the object in case it would be blocking on
			// access back to this object. (This is the case for the tracing class
			// since the thread owned by the tracing class also traces).
			// TODO(hellner): this is a bit out of place but here goes, de-couple
			// thread implementation with trace implementation.
			crit_sect->Leave();
			if (old_instance) {
				delete old_instance;
			}
			// Re-acquire the lock since the scoped critical section will release
			// it.
			crit_sect->Enter();
			return NULL;
		}
#else  // _WIN32
		if (count_operation ==
			kAddRefNoCreate && instance_count == 0)
		{
			return NULL;
		}
		if (count_operation == kAddRefNoCreate)
		{
			if (1 == InterlockedIncrement(&instance_count))
			{
				// The instance has been destroyed by some other thread. Rollback.
				InterlockedDecrement(&instance_count);
				assert(false);
				return NULL;
			}
			// Sanity to catch corrupt state.
			if (instance == NULL)
			{
				assert(false);
				InterlockedDecrement(&instance_count);
				return NULL;
			}
		}
		else if (count_operation == kAddRef)
		{
			if (instance_count == 0)
			{
				state = kCreate;
			}
			else {
				if (1 == InterlockedIncrement(&instance_count))
				{
					// InterlockedDecrement because reference count should not be
					// updated just yet (that's done when the instance is created).
					InterlockedDecrement(&instance_count);
					state = kCreate;
				}
			}
		}
		else
		{
			int new_value = InterlockedDecrement(&instance_count);
			if (new_value == 0)
			{
				state = kDestroy;
			}
		}

		if (state == kCreate)
		{
			// Create instance and let whichever thread finishes first assign its
			// local copy to the global instance. All other threads reclaim their
			// local copy.
			T* new_instance = T::CreateInstance();
			if (1 == InterlockedIncrement(&instance_count))
			{
				InterlockedExchangePointer(reinterpret_cast<void * volatile*>(&instance),
					new_instance);
			}
			else
			{
				InterlockedDecrement(&instance_count);
				if (new_instance)
				{
					delete static_cast<T*>(new_instance);
				}
			}
		}
		else if (state == kDestroy)
		{
			T* old_value = static_cast<T*>(InterlockedExchangePointer(
				reinterpret_cast<void * volatile*>(&instance), NULL));
			if (old_value)
			{
				delete static_cast<T*>(old_value);
			}
			return NULL;
		}
#endif  // #ifndef _WIN32
		return instance;
	}

}  // namspace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_STATIC_INSTANCE_H_
