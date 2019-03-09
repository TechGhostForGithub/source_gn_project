#include "modules/EngineModule/source/engine_module_impl.h"
#include <assert.h>

namespace gn
{
	CEngineModuleImpl::CEngineModuleImpl()
		: m_gnBase(NULL)
	{
	}

	CEngineModuleImpl::~CEngineModuleImpl()
	{
	}

	STDMETHODIMP_(int32_t) CEngineModuleImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceEngineModule, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CEngineModuleImpl::terminate(THIS_)
	{
		GN_TRACE(kTraceInfo, kTraceEngineModule, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceEngineModule, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

	STDMETHODIMP_(int32_t) CEngineModuleImpl::test(THIS_)
	{
		// do something
		return 0;
	}
}
