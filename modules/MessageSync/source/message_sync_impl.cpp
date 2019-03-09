#include "modules/MessageSync/source/message_sync_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib, "LogModule.dll.lib")

namespace gn
{
	CMessageSyncImpl::CMessageSyncImpl()
		: m_gnBase(NULL)
	{
	}

	CMessageSyncImpl::~CMessageSyncImpl()
	{
	}

	STDMETHODIMP_(int32_t) CMessageSyncImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceMessageSync, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CMessageSyncImpl::terminate(THIS_)
	{
		GN_TRACE(kTraceInfo, kTraceMessageSync, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceMessageSync, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

	STDMETHODIMP_(int32_t) CMessageSyncImpl::test(THIS_)
	{
		// do something
		return 0;
	}
}
