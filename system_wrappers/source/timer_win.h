/*
 *  Copyright (c) 2013 The cctalk project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef GN_SYSTEM_WRAPPERS_SOURCE_TIMER_WIN_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_TIMER_WIN_H_

#include <windows.h>
#include "system_wrappers/interface/timer_wrapper.h"
#include "system_wrappers/interface/critical_section_wrapper.h"
#include "typedefs.h"
#include <map>

namespace gn {
	struct TimerItem 
	{
		int32_t timerID;
		uint32_t event_timerID;
		void* timer;
		void* userData;
		LPTIMERCALLBACK callback;
	};
	
	typedef std::map<uint32_t,TimerItem*> TimerList;
	typedef TimerList::iterator TimerListItr;

	class TimerWinSec : public TimerWrapper {
	public:
		TimerWinSec(void);

		virtual ~TimerWinSec(void);

		virtual int32_t SetTimer(uint32_t periodicMS,
			LPTIMERCALLBACK callback, void* userData);

		virtual bool KillTimer(int32_t timerID);

		virtual bool IsTimer(int32_t timerID);

	private:
		void OnTimer(UINT_PTR idEvent);
		static VOID CALLBACK __TimeProc(
			_In_ HWND     hwnd,
			_In_ UINT     uMsg,
			_In_ UINT_PTR idEvent,
			_In_ DWORD    dwTime
			);

	private:
		TimerList				timerList_;
		CriticalSectionWrapper& crit_cs_;
	};

	class TimerWin : public TimerWrapper {
	public:
		TimerWin(void);

		virtual ~TimerWin(void);

		virtual int32_t SetTimer(uint32_t periodicMS,
			LPTIMERCALLBACK callback, void* userData);

		virtual bool KillTimer(int32_t timerID);
		virtual bool IsTimer(int32_t timerID);
	};

	
};  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_SOURCE_TIMER_WIN_H_
