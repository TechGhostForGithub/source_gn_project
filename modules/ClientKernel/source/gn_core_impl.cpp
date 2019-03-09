#include "modules/ClientKernel/source/gn_core_impl.h"
#include "modules/LogModule/log_module.h"
#include <assert.h>
#pragma warning(disable:4355)
namespace gn
{
	// Counter to be ensure that we can add a correct ID in all static trace
	// methods. It is not the nicest solution, especially not since we already
	// have a counter in GnCoreImpl. In other words, there is room for
	// improvement here.
	static int32_t gHjInstanceCounter = 0;

	GnCore* getGnCore()
	{
		GnCoreImpl* self = new GnCoreImpl();
		if (self != NULL)
		{
			// First reference.  Released in GnCore::Delete.
			self->addRef();
			gHjInstanceCounter++;
		}
		return self;
	}

	GnCoreImpl::GnCoreImpl(void)
		: GnBaseImpl(this)
		, m_ref_count(0)
	{
		m_gnCore = this;
	}

	GnCoreImpl::~GnCoreImpl()
	{
		m_gnCore = NULL;
		assert(m_ref_count.Value() == 0);
	}

	int32_t GnCoreImpl::addRef()
	{
		return ++m_ref_count;
	}

	// This implements the Release() method for all the inherited interfaces.
	int32_t GnCoreImpl::release()
	{
		int32_t new_ref = --m_ref_count;
		assert(new_ref >= 0);
		if (new_ref == 0) {
			GN_TRACE(kTraceApiCall, kTraceClientKernel, -1, "GnCoreImpl self deleting (GnCore=0x%p)",this);
			delete this;
		}

		return new_ref;
	}

	GnCore* GnCore::create()
	{
		return getGnCore();
	}

	bool GnCore::destroy(GnCore*& core)
	{
		if (core == NULL)
		{
			return false;
		}

		GnCoreImpl* s = static_cast<GnCoreImpl*>(core);

		// Release the reference that was added in GetVoiceEngine.
		int32_t ref = s->release();
		core = NULL;

		if (ref != 0) {
			// GnCore::Delete did not release the very last reference.
			assert(0);
		}

		return true;
	}
}

#pragma warning(default:4355)
