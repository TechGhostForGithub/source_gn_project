#include "modules/DataCenter/source/data_center_impl.h"
#include <assert.h>

namespace gn
{
	CDataCenterImpl::CDataCenterImpl()
        : m_gnBase(NULL)
	{
	}

	CDataCenterImpl::~CDataCenterImpl()
	{
		assert(NULL == m_gnBase);
	}

	STDMETHODIMP_(int32_t) CDataCenterImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceDataCenter, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CDataCenterImpl::terminate(THIS_)
	{
        GN_TRACE(kTraceInfo, kTraceDataCenter, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceDataCenter, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

    STDMETHODIMP_(int32_t) CDataCenterImpl::test(THIS_)
    {
        // do something
        return 0;
    }
}
