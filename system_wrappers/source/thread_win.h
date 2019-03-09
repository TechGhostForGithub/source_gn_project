#ifndef GN_SYSTEM_WRAPPERS_SOURCE_THREAD_WIN_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_THREAD_WIN_H_

#include "system_wrappers/interface/thread_wrapper.h"

#include <windows.h>

#include "system_wrappers/interface/critical_section_wrapper.h"
#include "system_wrappers/interface/event_wrapper.h"

namespace gn
{

	class ThreadWindows : public ThreadWrapper
	{
	public:
		ThreadWindows(ThreadRunFunction func, ThreadObj obj, ThreadPriority prio,
			const char* thread_name);
		virtual ~ThreadWindows();

		virtual bool Start(unsigned int& id);
		bool SetAffinity(const int* processor_numbers,
			const unsigned int amount_of_processors);
		virtual bool Stop();
		virtual void SetNotAlive();
		virtual bool Stop(bool& force);
        virtual void SetThreadAfterFunc(ThreadAfterFunction afterFunc);

		static unsigned int WINAPI StartThread(LPVOID lp_parameter);

	protected:
		virtual void Run();

	private:
		ThreadRunFunction    run_function_;
        ThreadAfterFunction after_function;
		ThreadObj            obj_;

		bool                    alive_;
		bool                    dead_;

		// TODO(hellner)
		// do_not_close_handle_ member seem pretty redundant. Should be able to remove
		// it. Basically it should be fine to reclaim the handle when calling stop
		// and in the destructor.
		bool                    do_not_close_handle_;
		ThreadPriority          prio_;
		EventWrapper*           event_;
		CriticalSectionWrapper* critsect_stop_;
		CriticalSectionWrapper* critsect_interface_;

		HANDLE                  thread_;
		unsigned int            id_;
		char                    name_[kThreadMaxNameLength];
		bool                    set_thread_name_;

	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_SOURCE_THREAD_WIN_H_
