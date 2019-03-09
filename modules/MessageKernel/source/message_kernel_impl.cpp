#include "modules/MessageKernel/source/message_kernel_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib, "LogModule.dll.lib")

namespace gn
{
	CMessageKernel::CMessageKernel()
		: m_gnBase(NULL)
	{
	}

	CMessageKernel::~CMessageKernel()
	{
	}

	STDMETHODIMP_(int32_t) CMessageKernel::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceMessageKernel, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CMessageKernel::terminate(THIS_)
	{
		GN_TRACE(kTraceInfo, kTraceMessageKernel, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceMessageKernel, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

	STDMETHODIMP_(int32_t) CMessageKernel::test(THIS_)
	{
		// do something
		return 0;
	}
}
