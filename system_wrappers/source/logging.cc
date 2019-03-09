﻿/*
 *  Copyright (c) 2012 The cctalk project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "system_wrappers/interface/logging.h"

#include <string.h>

#include <sstream>

#include "common_types.h"
#include "system_wrappers/interface/trace.h"

namespace gn {
namespace {

TraceLevel WebRtcSeverity(LoggingSeverity sev) {
  switch (sev) {
    // TODO(ajm): SENSITIVE doesn't have a corresponding hj level.
    case LS_SENSITIVE:  return kTraceInfo;
    case LS_VERBOSE:    return kTraceInfo;
    case LS_INFO:       return kTraceTerseInfo;
    case LS_WARNING:    return kTraceWarning;
    case LS_ERROR:      return kTraceError;
    default:            return kTraceNone;
  }
}

const char* DescribeFile(const char* file) {
  const char* end1 = ::strrchr(file, '/');
  const char* end2 = ::strrchr(file, '\\');
  if (!end1 && !end2)
    return file;
  else
    return (end1 > end2) ? end1 + 1 : end2 + 1;
}

}  // namespace

LogMessage::LogMessage(const char* file, int line, LoggingSeverity sev)
    : severity_(sev) {
  print_stream_ << "(" << DescribeFile(file) << ":" << line << "): ";
}

bool LogMessage::Loggable(LoggingSeverity sev) {
  // |level_filter| is a bitmask, unlike libjingle's minimum severity value.
  return WebRtcSeverity(sev) & Trace::level_filter() ? true : false;
}

LogMessage::~LogMessage() {
  const std::string& str = print_stream_.str();
  Trace::Add(WebRtcSeverity(severity_), kTraceUndefined, 0, str.c_str());
}

}  // namespace gn
