#ifndef _TS_BASE_H_
#define _TS_BASE_H_

#include "comdefs.h"
#include "common_types.h"

namespace gn
{
	// GnCore
	class GN_DLLEXPORT GnCore
	{
	public:
		// Creates a GnCore object, which can then be used to acquire
		// sub-APIs. Returns NULL on failure. 
		static GnCore* create();

		// Deletes a created GnCore object and releases the utilized resources.
		// Note that if there are outstanding references held via other interfaces,
		// the GnCore instance will not actually be deleted until those
		// references have been released.
		static bool destroy(GnCore*& core);

		// test
		static void test();

	protected:
		GnCore() {}
		virtual ~GnCore() {}
	};

	// GnBase
	class GN_DLLEXPORT GnBase
	{
	public:
		// Factory for the GnBase sub-API. Increases an internal reference
		// counter if successful. Returns NULL if the API is not supported or if
		// construction fails.
		static GnBase* getInterface(GnCore* core);

		// Releases the GnBase sub-API and decreases an internal reference
		// counter. Returns the new reference count. This value should be zero
		// for all sub-APIs before the GnCore object can be safely deleted.
		virtual int32_t release() = 0;

		// Initializes all common parts of the GnCore; e.g. all
		// encoders/decoders, the sound card and core receiving components.
		// If NULL is passed for any of these, GnCore will create its own.
		// TODO(ajm): Remove default NULLs.
		virtual int32_t init() = 0;

		// Terminates all GnCore functions and releses allocated resources.
		virtual int32_t terminate() = 0;

		// Query moudle interface
		virtual int32_t queryInterface(REFCLSID rclsid,REFIID riid,void** ppv) = 0;

	protected:
		GnBase() {}
		virtual ~GnBase() {}
	};
}
#endif // _TS_BASE_H_
