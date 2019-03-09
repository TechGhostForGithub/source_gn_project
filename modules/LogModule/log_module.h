#ifndef _LOG_MODULE_H_
#define _LOG_MODULE_H_
#include "comdefs.h"
#include "common_types.h"
#include "system_wrappers/interface/trace.h"

namespace gn
{
#define GN_TRACE GnLog::add
	class GN_DLLEXPORT GnLog
	{
	public:
		static void createTrace();
		static int setTraceFilter(unsigned int filter);
		static int setTraceFile(const char* fileNameUTF8, bool addFileCounter);
		static int setTraceCallback(TraceCallback* callback);
		static void add(const TraceLevel level, const TraceModule module, const int32_t id, const char* msg, ...);
	};

}

#endif // _LOG_MODULE_H_
