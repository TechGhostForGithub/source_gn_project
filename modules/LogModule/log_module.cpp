#include "log_module.h"
#include <list>
#include <string>
using namespace std;
#include "common_types.h"
#include "system_wrappers/interface/atomic32.h"
#include "system_wrappers/interface/critical_section_wrapper.h"
#include "system_wrappers/interface/event_wrapper.h"
#include "system_wrappers/source/trace_impl.h"
#ifdef CCORE_ANDROID
#include "system_wrappers/interface/logcat_trace_context.h"
#endif
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"system_wrappers.lib")

namespace gn
{
	void GnLog::createTrace()
	{
		Trace::CreateTrace();
#ifdef CCORE_ANDROID
		static LogcatTraceContext instance = LogcatTraceContext();
#endif
	}

	int GnLog::setTraceFilter(unsigned int filter)
	{
		GN_TRACE1(kTraceApiCall, kTraceClientKernel,0, "SetTraceFilter(filter=0x%x)", filter);

		// Remember old filter
		uint32_t oldFilter = Trace::level_filter();
		Trace::set_level_filter(filter);

		// If previous log was ignored, log again after changing filter
		if (kTraceNone == oldFilter)
		{
			GN_TRACE1(kTraceApiCall, kTraceClientKernel, -1, "SetTraceFilter(filter=0x%x)", filter);
		}

		return 0;
	}

	int GnLog::setTraceFile(const char* fileNameUTF8, bool addFileCounter)
	{
		int ret = Trace::SetTraceFile(fileNameUTF8, addFileCounter);
		GN_TRACE1(kTraceApiCall, kTraceClientKernel,0, "SetTraceFile(fileNameUTF8=%s, addFileCounter=%d)", fileNameUTF8, addFileCounter);
		return (ret);
	}

	int GnLog::setTraceCallback(TraceCallback* callback)
	{
		GN_TRACE1(kTraceApiCall, kTraceClientKernel,0, "SetTraceCallback(callback=0x%x)", callback);
		return (Trace::SetTraceCallback(callback));
	}

	void GnLog::add(const TraceLevel level, const TraceModule module, const int32_t id, const char* msg, ...)
	{
		TraceImpl* trace = TraceImpl::GetTrace(level);
		if (trace) {
			if (trace->TraceCheck(level)) {
				char temp_buff[GN_TRACE_MAX_MESSAGE_SIZE];
				char* buff = 0;
				if (msg) {
					va_list args;
					va_start(args, msg);
#ifdef _MSC_VER
					_vsnprintf_s(temp_buff, GN_TRACE_MAX_MESSAGE_SIZE - 1, msg, args);
#else
					vsnprintf(temp_buff, GN_TRACE_MAX_MESSAGE_SIZE - 1, msg, args);
#endif
					va_end(args);
					buff = temp_buff;
				}
				trace->AddImpl(level, module, id, buff);
			}
			Trace::ReturnTrace();
		}
	}
}
