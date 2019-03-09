#include "modules/CommonHead/source/common_head_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib, "LogModule.dll.lib")
#include "assert.h"

namespace gn
{
	CCommonHeadImpl::CCommonHeadImpl()
		: m_gnBase(NULL)
	{

	}

	CCommonHeadImpl::~CCommonHeadImpl()
	{
	}

	STDMETHODIMP_(int32_t) CCommonHeadImpl::init(THIS_ GnBase* base)
	{      
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceCommonHead, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CCommonHeadImpl::terminate(THIS_ )
	{
		GN_TRACE(kTraceInfo, kTraceCommonHead, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceCommonHead, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

	STDMETHODIMP_(int32_t) CCommonHeadImpl::registerCommonHeadObserver(THIS_ ICommonHeadObserver* observer)
	{
		// do something
		return 0;
	}

	STDMETHODIMP_(int32_t) CCommonHeadImpl::deRegisterCommonHeadObserver(THIS_ ICommonHeadObserver* observer)
	{
		// do something
		return 0;
	}

	STDMETHODIMP_(int32_t) CCommonHeadImpl::test(THIS_)
	{
		// do something
		return 0;
	}

	STDMETHODIMP_(void) CCommonHeadImpl::onHttpTest(THIS_)
	{
		// do something
	}
}
