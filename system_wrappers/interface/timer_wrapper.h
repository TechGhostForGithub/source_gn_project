/*
 *  Copyright (c) 2013 The cctalk project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_TIMER_WRAPPER_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_TIMER_WRAPPER_H_
#include "typedefs.h"

namespace gn {

	typedef void (TIMERCALLBACK)(int32_t timerID, void* userData);
	typedef TIMERCALLBACK *LPTIMERCALLBACK;

	class TimerWrapper {
	public:
		// Factory method, constructor disabled
		static TimerWrapper* CreateTimer();

		virtual ~TimerWrapper() {}

		// Return timer id
		virtual int32_t SetTimer(uint32_t periodicMS,
			LPTIMERCALLBACK callback, void* userData) = 0;

		// kill timer
		virtual bool KillTimer(int32_t timerID) = 0;

		// timer id is timer
		virtual bool IsTimer(int32_t timerID) = 0;
	};

};  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_TIMER_WRAPPER_H_
