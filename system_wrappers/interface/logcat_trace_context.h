/*
 *  Copyright (c) 2013 The cctalk project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_LOGCAT_TRACE_CONTEXT_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_LOGCAT_TRACE_CONTEXT_H_

#include "system_wrappers/interface/trace.h"

#ifndef __ANDROID__
#error This file only makes sense to include on Android!
#endif

namespace gn {

// Scoped helper class for directing Traces to Android's logcat facility.  While
// this object lives, Trace output will be sent to logcat.
class LogcatTraceContext : public gn::TraceCallback {
 public:
  LogcatTraceContext();
  virtual ~LogcatTraceContext();

  // TraceCallback impl.
  virtual void print(TraceLevel level, const char* message, int length);
};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_LOGCAT_TRACE_CONTEXT_H_
