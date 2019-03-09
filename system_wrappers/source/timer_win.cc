#include "system_wrappers/source/timer_win.h"
#include "system_wrappers/interface/trace.h"
#include "modules/LogModule/log_module.h"
#include <assert.h>

namespace gn {

	static int32_t g_timer_id_index_ = 0;
	static TimerWinSec g_timer_win_sec_;

	TimerWin::TimerWin(void)
	{

	}

	TimerWin::~TimerWin(void)
	{

	}

	int32_t TimerWin::SetTimer(uint32_t periodicMS,
		LPTIMERCALLBACK callback, void* userData)
	{
		return g_timer_win_sec_.SetTimer(periodicMS,callback,userData);
	}

	bool TimerWin::KillTimer(int32_t timerID)
	{
		return g_timer_win_sec_.KillTimer(timerID);
	}

	bool TimerWin::IsTimer(int32_t timerID)
	{
		return g_timer_win_sec_.IsTimer(timerID);
	}

	TimerWinSec::TimerWinSec(void)
		: crit_cs_(*CriticalSectionWrapper::CreateCriticalSection())
	{
	}

	TimerWinSec::~TimerWinSec(void)
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

	VOID TimerWinSec::__TimeProc(
		_In_ HWND     hwnd,
		_In_ UINT     uMsg,
		_In_ UINT_PTR idEvent,
		_In_ DWORD    dwTime
		)
	{
		g_timer_win_sec_.OnTimer(idEvent);
	}

	void TimerWinSec::OnTimer(UINT_PTR idEvent)
	{
		int32_t timerID = 0;
		void* userData = NULL;
		LPTIMERCALLBACK callback = NULL;
		{
			CriticalSectionScoped cs(&crit_cs_);
			for (TimerListItr it = timerList_.begin(); it != timerList_.end(); ++it)
			{
				if (it->second && it->second->callback && it->second->event_timerID == idEvent)
				{
					timerID = it->second->timerID;
					userData = it->second->userData;
					callback = it->second->callback;
				}
			}
		}
		if (callback)
		{
			callback(timerID,userData);
		}
	}

	int32_t TimerWinSec::SetTimer(uint32_t periodicMS,
		LPTIMERCALLBACK callback, void* userData)
	{
		CriticalSectionScoped cs(&crit_cs_);
		int32_t timerID = g_timer_id_index_++;
		TimerListItr it = timerList_.find(timerID);
		if (it != timerList_.end())
		{
			GN_TRACE(kTraceStateInfo, kTraceUtility, 0,
				"SetTimer with id:%d already exist,kill it.", it->first);
			BOOL t = ::KillTimer(NULL,it->second->event_timerID);
			if (t != TRUE)
			{
				GN_TRACE(kTraceError, kTraceUtility, 0,
					"SetTimer with id:%d kill failed ", timerID);
			}
			delete it->second;
			timerList_.erase(it);
		}

		TimerItem* item = new TimerItem;;
		item->timer = this;
		item->timerID = timerID;
		item->userData = userData;
		item->callback = callback;
		item->event_timerID = ::SetTimer(NULL,(UINT_PTR)item,periodicMS,__TimeProc);

		if (item->event_timerID == 0)
		{
			delete item;
			GN_TRACE(kTraceError, kTraceUtility, 0,
				"SetTimer with id:%d set failed ", timerID);
			return -1;
		}

		timerList_[timerID] = item;

		return timerID;
	}

	bool TimerWinSec::KillTimer(int32_t timerID)
	{
		CriticalSectionScoped cs(&crit_cs_);
		TimerListItr it = timerList_.find(timerID);
		if (it != timerList_.end())
		{
			BOOL t = ::KillTimer(NULL,it->second->event_timerID);
			if (t != TRUE)
			{
				GN_TRACE(kTraceError, kTraceUtility, 0,
					"KillTimer with id:%d kill failed ", timerID);
			}
			delete it->second;
			timerList_.erase(it);
		}

		return true;
	}

	bool TimerWinSec::IsTimer(int32_t timerID)
	{
		CriticalSectionScoped cs(&crit_cs_);
		TimerListItr it = timerList_.find(timerID);

		return it != timerList_.end();
	}

}
