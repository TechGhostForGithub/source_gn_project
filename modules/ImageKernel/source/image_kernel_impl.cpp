
#include "modules/ImageKernel/source/image_kernel_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib, "LogModule.dll.lib")

namespace gn
{
	CImageKernelImpl::CImageKernelImpl()
		: m_gnBase(NULL)
	{

	}

	CImageKernelImpl::~CImageKernelImpl()
	{
	}

	STDMETHODIMP_(int32_t) CImageKernelImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceImageKernel, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CImageKernelImpl::terminate(THIS_ )
	{
		GN_TRACE(kTraceInfo, kTraceImageKernel, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceImageKernel, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

	STDMETHODIMP_(int32_t) CImageKernelImpl::test(THIS_)
	{
		return 0;
	}
}
