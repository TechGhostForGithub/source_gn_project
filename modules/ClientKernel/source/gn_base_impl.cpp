#include "modules/ClientKernel/source/gn_base_impl.h"
#include "modules/ClientKernel/source/gn_core_impl.h"
#include "modules/ClientKernel/source/gn_shared_data.h"
#include "modules/LogModule/log_module.h"

namespace gn
{
	// ---------------------------------------------
	// gn_base.h
	// GnBase* GnBase::getInterface(GnCore* core);
	// ---------------------------------------------
	GnBase* GnBase::getInterface(GnCore* core)
	{
		if (NULL == core) {
			return NULL;
		}
		GnCoreImpl* s = static_cast<GnCoreImpl*>(core);
		s->addRef();
		return s;
	}

	// ---------------------------------------------
	// gn_base_impl.h
	// ---------------------------------------------
	GnBaseImpl::GnBaseImpl(KernelSharedData* shared)
		: m_sharedData(shared)
	{
	}

	GnBaseImpl::~GnBaseImpl()
	{
	}

	int32_t GnBaseImpl::init()
	{
		if (m_modules.loadResource() == -1) {
			GN_TRACE(kTraceError, kTraceClientKernel, 0, "%s load resource failed",__FUNCTION__);
			return -1;
		}

		return 0;
	}

	int32_t GnBaseImpl::terminate()
	{	
		if (m_modules.unloadResource() == -1) {
			GN_TRACE(kTraceWarning, kTraceClientKernel, 0, "%s unload resource failed",__FUNCTION__);
			return -1;
		}

		return 0;
	}

	int32_t GnBaseImpl::queryInterface(REFCLSID rclsid,REFIID riid,void** ppv)
	{
		return m_modules.queryInterface(rclsid,riid,ppv);
	}

}  // namespace gn
