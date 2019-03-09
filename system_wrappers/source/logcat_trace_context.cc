/*
 *  Copyright (c) 2013 The cctalk project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "system_wrappers/interface/logcat_trace_context.h"

#include <android/log.h>
#include <assert.h>

#include "system_wrappers/interface/logging.h"

namespace gn {

static android_LogPriority AndroidLogPriorityFromWebRtcLogLevel(
    TraceLevel webrtc_level) {
  // NOTE: this mapping is somewhat arbitrary.  StateInfo and Info are mapped
  // to DEBUG because they are highly verbose in hj code (which is
  // unfortunate).
  switch (webrtc_level) {
    case gn::kTraceStateInfo: return ANDROID_LOG_DEBUG;
    case gn::kTraceWarning: return ANDROID_LOG_WARN;
    case gn::kTraceError: return ANDROID_LOG_ERROR;
    case gn::kTraceCritical: return ANDROID_LOG_FATAL;
    case gn::kTraceApiCall: return ANDROID_LOG_VERBOSE;
    case gn::kTraceModuleCall: return ANDROID_LOG_VERBOSE;
    case gn::kTraceMemory: return ANDROID_LOG_VERBOSE;
    case gn::kTraceTimer: return ANDROID_LOG_VERBOSE;
    case gn::kTraceStream: return ANDROID_LOG_VERBOSE;
    case gn::kTraceDebug: return ANDROID_LOG_DEBUG;
    case gn::kTraceInfo: return ANDROID_LOG_DEBUG;
    case gn::kTraceTerseInfo: return ANDROID_LOG_INFO;
    default:
      LOG(LS_ERROR) << "Unexpected log level" << webrtc_level;
      return ANDROID_LOG_FATAL;
  }
}

LogcatTraceContext::LogcatTraceContext() {
  gn::Trace::CreateTrace();
  if (gn::Trace::SetTraceCallback(this) != 0)
    assert(false);
}

LogcatTraceContext::~LogcatTraceContext() {
  if (gn::Trace::SetTraceCallback(NULL) != 0)
    assert(false);
  gn::Trace::ReturnTrace();
}

void LogcatTraceContext::print(TraceLevel level,
                               const char* message,
                               int length) {
  __android_log_print(AndroidLogPriorityFromWebRtcLogLevel(level),
                      "GN", "%.*s", length, message);
}

}  // namespace gn
