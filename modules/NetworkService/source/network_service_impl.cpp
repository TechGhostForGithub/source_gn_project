#include <assert.h>
#include "modules/NetworkService/source/network_service_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib, "LogModule.dll.lib")

namespace gn
{
	CNetworkServiceImpl::CNetworkServiceImpl()
		: m_gnBase(NULL)
	{
	}

	CNetworkServiceImpl::~CNetworkServiceImpl()
	{
	}

	STDMETHODIMP_(int32_t) CNetworkServiceImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceNetworkService, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CNetworkServiceImpl::terminate(THIS_)
	{
		GN_TRACE(kTraceInfo, kTraceNetworkService, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceNetworkService, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;

		return 0;
	}

	STDMETHODIMP_(int32_t) CNetworkServiceImpl::test(THIS_)
	{
		// do something
		return 0;
	}
}
