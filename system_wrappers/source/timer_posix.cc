#include "system_wrappers/source/timer_posix.h"
#include "system_wrappers/interface/trace.h"
#include "modules/LogModule/log_module.h"
#include <string.h>
#include <stdio.h>

namespace gn {

	static int32_t g_timer_id_index_ = 0;

	TimerPosix::TimerPosix(void)
		: crit_cs_(*CriticalSectionWrapper::CreateCriticalSection())
	{
	}

	TimerPosix::~TimerPosix(void)
	{
		{
			CriticalSectionScoped cs(&crit_cs_);
			if (timerList_.size() > 0)
			{
				for (TimerListItr it = timerList_.begin();
					it != timerList_.end(); ++it)
				{
					delete it->second;
				}
				timerList_.clear();
				GN_TRACE(kTraceWarning, kTraceUtility, 0,
					"Timer list is not empty.");
			}
		}
		delete &crit_cs_;
	}

	void TimerPosix::__TimeProc(int sig,siginfo_t *si,void *uc)
	{
#ifdef __ANDROID__
	    if (sig == SIGALRM)
#else
		if (sig == SIGUSR1)
#endif
	    {
	        if (si)
	        {
                TimerItem* item = (TimerItem*)si->si_value.sival_ptr;
                if (item && item->timer)
                {
                    ((TimerPosix*)(item->timer))->OnTimer(item);
                }
	        }
	    }
	}

	void TimerPosix::OnTimer(TimerItem* item)
	{
		int32_t timerID = 0;
		void* userData = NULL;
		LPTIMERCALLBACK callback = NULL;
		{
			CriticalSectionScoped cs(&crit_cs_);
			TimerListItr it = timerList_.find(item->timerID);
			if (it != timerList_.end())
			{
				if (item->callback)
				{
					timerID = item->timerID;
					userData = item->userData;
					callback = item->callback;
				}
			}
		}
		if (callback)
		{
			callback(timerID,userData);
		}
	}

	int32_t TimerPosix::SetTimer(uint32_t periodicMS,
		LPTIMERCALLBACK callback, void* userData)
	{
		CriticalSectionScoped cs(&crit_cs_);
		int32_t timerID = g_timer_id_index_++;
		TimerListItr it = timerList_.find(timerID);
		if (it != timerList_.end())
		{
			GN_TRACE(kTraceStateInfo, kTraceUtility, 0,
				"SetTimer with id:%d already exist,kill it.", it->first);
            if (timer_delete(it->second->event_timerID) == -1)
			{
				GN_TRACE(kTraceError, kTraceUtility, 0,
					"SetTimer timer_delete with id:%d kill failed ", timerID);
			}
			delete it->second;
			timerList_.erase(it);
		}

		TimerItem* item = new TimerItem;;
		item->timer = this;
		item->timerID = timerID;
		item->userData = userData;
		item->callback = callback;

        struct sigaction act;
        memset(&act, 0, sizeof(act));
        sigemptyset(&act.sa_mask);
        act.sa_sigaction = __TimeProc;
        act.sa_flags = SA_SIGINFO;
#ifdef __ANDROID__
        if (sigaction(SIGALRM, &act, NULL) == -1)
#else
		if (sigaction(SIGUSR1, &act, NULL) == -1)
#endif
        {
			delete item;
			GN_TRACE(kTraceError, kTraceUtility, 0,
				"SetTimer sigaction with id:%d set failed ", timerID);
            return -1;
        }
		struct sigevent sev;
		memset((void*)&sev, 0, sizeof (sev));
		sev.sigev_notify = SIGEV_SIGNAL;
#ifdef __ANDROID__
		sev.sigev_signo = SIGALRM;
#else
		sev.sigev_signo = SIGUSR1;
#endif
		//sev.sigev_notify = SIGEV_THREAD_ID;
		//sev._sigev_un._tid = pthread_self ();
		//sev.sigev_notify_function = __TimeProc;
		sev.sigev_value.sival_int = (int32_t)timerID;
		sev.sigev_value.sival_ptr = item;

		if (timer_create (CLOCK_REALTIME, &sev, &item->event_timerID) == -1)
		{
			delete item;
			GN_TRACE(kTraceError, kTraceUtility, 0,
				"SetTimer timer_create with id:%d set failed ", timerID);
			return -1;
		}

        struct itimerspec its;
        its.it_value.tv_sec = periodicMS / 1000;
        its.it_value.tv_nsec = (periodicMS % 1000) * 1000000;
        its.it_interval.tv_sec = periodicMS / 1000;
        its.it_interval.tv_nsec = (periodicMS % 1000) * 1000000;
        if (timer_settime (item->event_timerID, 0, &its, NULL) == -1)
        {
			delete item;
			GN_TRACE(kTraceError, kTraceUtility, 0,
				"SetTimer timer_settime with id:%d set failed ", timerID);
			return -1;
        }

		timerList_[timerID] = item;

		return timerID;
	}

	bool TimerPosix::KillTimer(int32_t timerID)
	{
		CriticalSectionScoped cs(&crit_cs_);
		TimerListItr it = timerList_.find(timerID);
		if (it != timerList_.end())
		{
			if (timer_delete(it->second->event_timerID) == -1)
			{
                delete it->second;
				timerList_.erase(it);
				GN_TRACE(kTraceError, kTraceUtility, 0,
					"KillTimer timer_delete with id:%d set failed ", timerID);
			    return false;
			}
			delete it->second;
			timerList_.erase(it);
		}

		return true;
	}

	bool TimerPosix::IsTimer(int32_t timerID)
	{
		CriticalSectionScoped cs(&crit_cs_);
		TimerListItr it = timerList_.find(timerID);

		return it != timerList_.end();
	}

}
