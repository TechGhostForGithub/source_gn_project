/*
 *  Copyright (c) 2012 The cctalk project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef GN_SYSTEM_WRAPPERS_SOURCE_THREAD_POSIX_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_THREAD_POSIX_H_

#include "system_wrappers/interface/thread_wrapper.h"

#include <pthread.h>

namespace gn {

class CriticalSectionWrapper;
class EventWrapper;

int ConvertToSystemPriority(ThreadPriority priority, int min_prio,
                            int max_prio);

class ThreadPosix : public ThreadWrapper {
 public:
  static ThreadWrapper* Create(ThreadRunFunction func, ThreadObj obj,
                               ThreadPriority prio, const char* thread_name);

  ThreadPosix(ThreadRunFunction func, ThreadObj obj, ThreadPriority prio,
              const char* thread_name);
  virtual ~ThreadPosix();

  // From ThreadWrapper.
  virtual void SetNotAlive() OVERRIDE;
  virtual bool Start(unsigned int& id) OVERRIDE;
  // Not implemented on Mac.
  virtual bool SetAffinity(const int* processor_numbers,
                           unsigned int amount_of_processors) OVERRIDE;
  virtual bool Stop() OVERRIDE;
  virtual bool Stop(bool& force) OVERRIDE;
  virtual void SetThreadAfterFunc(ThreadAfterFunction afterFunc) OVERRIDE;


  void Run();

 private:
  int Construct();

 private:
  ThreadRunFunction   run_function_;
  ThreadAfterFunction after_function;
  ThreadObj           obj_;

  // Internal state.
  CriticalSectionWrapper* crit_state_;  // Protects alive_ and dead_
  bool                    alive_;
  bool                    dead_;
  ThreadPriority          prio_;
  EventWrapper*           event_;

  // Zero-terminated thread name string.
  char                    name_[kThreadMaxNameLength];
  bool                    set_thread_name_;

  // Handle to thread.
#if (defined(CCORE_LINUX) || defined(CCORE_ANDROID))
  pid_t                   pid_;
#endif
  pthread_attr_t          attr_;
  pthread_t               thread_;
};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_SOURCE_THREAD_POSIX_H_
