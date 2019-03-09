#ifndef _COMMON_TYPES_H_
#define _COMMON_TYPES_H_

#include "typedefs.h"

#if defined(_MSC_VER)
// Disable "new behavior: elements of array will be default initialized"
// warning. Affects OverUseDetectorOptions.
#pragma warning(disable:4351)
#endif

#ifdef GN_EXPORT
#define GN_DLLEXPORT _declspec(dllexport)
#elif GN_DLL
#define GN_DLLEXPORT _declspec(dllimport)
#else
#define GN_DLLEXPORT
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef SAFE_DELETE_PTR(x)
#define SAFE_DELETE_PTR(x) if ((x)) { delete (x); }
#endif

#ifndef SAFE_DELETE_ARRAY(x)
#define SAFE_DELETE_ARRAY(x) if ((x)) { delete[] (x); }
#endif

namespace gn
{
	class InStream
	{
	public:
		virtual int Read(void *buf, int len) = 0;
		virtual int Rewind() {return -1;}
		virtual ~InStream() {}
	protected:
		InStream() {}
	};

	class OutStream
	{
	public:
		virtual bool Write(const void *buf,int len) = 0;
		virtual int Rewind() {return -1;}
		virtual ~OutStream() {}
	protected:
		OutStream() {}
	};

	enum TraceModule
	{
		kTraceUndefined          = 0,
		kTraceNetworkService	 = 0x0001,
		kTraceMessageSync		 = 0x0002,
		kTraceMessageKernel      = 0x0003,
		kTraceClientKernel	     = 0x0004,
		kTraceFileModule	     = 0x0005,
		kTraceFileTransfer		 = 0x0006,
		kTraceUserFace			 = 0x0007,
		kTraceDataCenter         = 0x0008,
		kTraceImageKernel		 = 0x0009,
		kTraceEngineModule       = 0x000a,
		kTraceDataReport         = 0x000b,
		kTraceNetworkRouting     = 0x000c,
		kTraceCommonHead		 = 0x000d,
        kTraceH5OfflinePkg       = 0x000e,
		kTraceHttpModule		 = 0x0010,
        kTraceHttpKernel         = 0x0011,
        kTraceCCore              = 0x0012,
		kTraceUtility			 = 0x0013,
	};

	enum TraceLevel
	{
		kTraceNone               = 0x0000,    // no trace
		kTraceStateInfo          = 0x0001,
		kTraceWarning            = 0x0002,
		kTraceError              = 0x0004,
		kTraceCritical           = 0x0008,
		kTraceApiCall            = 0x0010,
		kTraceDefault            = 0x00ff,

		kTraceModuleCall         = 0x0020,
		kTraceMemory             = 0x0100,   // memory info
		kTraceTimer              = 0x0200,   // timing info
		kTraceStream             = 0x0400,   // "continuous" stream of data

		// used for debug purposes
		kTraceDebug              = 0x0800,  // for positioning problem
		kTraceInfo               = 0x1000,  // debug info

		// Non-verbose level used by LS_INFO of logging.h. Do not use directly.
		kTraceTerseInfo          = 0x2000,

		kTraceAll                = 0xffff
	};

	// External Trace API
	class TraceCallback {
	public:
		virtual void print(TraceLevel level, const char* message, int length) = 0;

	protected:
		virtual ~TraceCallback() {}
		TraceCallback() {}
	};
}

#endif  // _COMMON_TYPES_H_

