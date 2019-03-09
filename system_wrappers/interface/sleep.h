// An OS-independent sleep function.

#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_SLEEP_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_SLEEP_H_

namespace gn
{

	// This function sleeps for the specified number of milliseconds.
	// It may return early if the thread is woken by some other event,
	// such as the delivery of a signal on Unix.
	void SleepMs(int msecs);

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_SLEEP_H_
