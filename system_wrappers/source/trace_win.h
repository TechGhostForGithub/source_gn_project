#ifndef GN_SYSTEM_WRAPPERS_SOURCE_TRACE_WIN_H_
#define GN_SYSTEM_WRAPPERS_SOURCE_TRACE_WIN_H_

#include <stdio.h>
#include <windows.h>

#include "system_wrappers/source/trace_impl.h"

namespace gn
{

	class TraceWindows : public TraceImpl
	{
	public:
		TraceWindows();
		virtual ~TraceWindows();

		virtual int32_t AddTime(char* trace_message, const TraceLevel level) const;

		virtual int32_t AddBuildInfo(char* trace_message) const;
		virtual int32_t AddDateTimeInfo(char* trace_message) const;
	private:
		volatile mutable uint32_t prev_api_tick_count_;
		volatile mutable uint32_t prev_tick_count_;
	};

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_SOURCE_TRACE_WIN_H_
