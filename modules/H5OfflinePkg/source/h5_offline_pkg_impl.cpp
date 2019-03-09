#include "modules/H5OfflinePkg/source/h5_offline_pkg_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib,"LogModule.dll.lib")                    

namespace gn
{
	CH5OfflinePkgImpl::CH5OfflinePkgImpl()
        : m_gnBase(NULL)
	{
	}

	CH5OfflinePkgImpl::~CH5OfflinePkgImpl()
	{
	}

    STDMETHODIMP_(int32_t) CH5OfflinePkgImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceH5OfflinePkg, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CH5OfflinePkgImpl::terminate(THIS_ )
	{
		GN_TRACE(kTraceInfo, kTraceH5OfflinePkg, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceH5OfflinePkg, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

    STDMETHODIMP_(int32_t) CH5OfflinePkgImpl::test(THIS_)
    {
        // do something
        return 0;
    }
}
