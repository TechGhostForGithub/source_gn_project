/*
 *  Copyright (c) 2013 The cctalk project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef GN_SYSTEM_WRAPPERS_SOURCE_TIMER_OC_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_TIMER_OC_H_

#include "system_wrappers/interface/timer_wrapper.h"
#include "system_wrappers/interface/critical_section_wrapper.h"
#include "typedefs.h"
#include <map>

namespace gn {

	typedef std::map<uint32_t,void*> TimerList;
	typedef TimerList::iterator TimerListItr;

	class TimerOc : public TimerWrapper {
	public:
		TimerOc(void);

		virtual ~TimerOc(void);

		virtual int32_t SetTimer(uint32_t periodicMS,
			LPTIMERCALLBACK callback, void* userData);

		virtual bool KillTimer(int32_t timerID);

		virtual bool IsTimer(int32_t timerID);

		void OnTimer(void* item);

	private:
		TimerList				timerList_;
		CriticalSectionWrapper& crit_cs_;
	};

}

#endif	// GN_SYSTEM_WRAPPERS_SOURCE_TIMER_OC_H_
