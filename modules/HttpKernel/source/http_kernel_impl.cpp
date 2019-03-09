#include "modules/HttpKernel/source/http_kernel_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib, "LogModule.dll.lib")
#include <assert.h>

namespace gn
{
	CHttpKernel::CHttpKernel()
        : m_gnBase(NULL)
	{
	}

	CHttpKernel::~CHttpKernel()
	{
		assert(NULL == m_gnBase);
	}

	STDMETHODIMP_(int32_t) CHttpKernel::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceHttpKernel, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CHttpKernel::terminate(THIS_)
	{
        GN_TRACE(kTraceInfo, kTraceHttpKernel, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceHttpKernel, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

    STDMETHODIMP_(int32_t) CHttpKernel::registerHttpKernelObserver(THIS_ IHttpKernelObserver* observer)
    {
        return 0;
    }

    STDMETHODIMP_(int32_t) CHttpKernel::deRegisterHttpKernelObserver(THIS_ IHttpKernelObserver* observer)
    {
        return 0;
    }

    STDMETHODIMP_(int32_t) CHttpKernel::test(THIS_)
    {
        // do something
        return 0;
    }
}
