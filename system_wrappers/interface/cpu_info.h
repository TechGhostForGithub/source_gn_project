/*
 *  Copyright (c) 2011 The cctalk project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_CPU_INFO_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_CPU_INFO_H_

#include "typedefs.h"

namespace gn {

class CpuInfo {
 public:
  static uint32_t DetectNumberOfCores();

 private:
  CpuInfo() {}
  static uint32_t number_of_cores_;
};

}  // namespace gn

#endif // GN_SYSTEM_WRAPPERS_INTERFACE_CPU_INFO_H_
