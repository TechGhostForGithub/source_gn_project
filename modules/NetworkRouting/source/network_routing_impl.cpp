#include "modules/NetworkRouting/source/network_routing_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib, "LogModule.dll.lib")
#include <assert.h>

namespace gn
{
	CNetworkRoutingImpl::CNetworkRoutingImpl()
		: m_gnBase(NULL)
	{
	}

	CNetworkRoutingImpl::~CNetworkRoutingImpl()
	{
	}

	STDMETHODIMP_(int32_t) CNetworkRoutingImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceNetworkRouting, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CNetworkRoutingImpl::terminate(THIS_)
	{
		GN_TRACE(kTraceInfo, kTraceNetworkRouting, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceNetworkRouting, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

	STDMETHODIMP_(int32_t) CNetworkRoutingImpl::test(THIS_)
	{
		// do something
		return 0;
	}
}  // namespace gn
