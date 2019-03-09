
#include "modules/DataReport/source/data_report_impl.h"
#include <assert.h>

namespace gn
{
	CDataReportImpl::CDataReportImpl()
		: m_gnBase(NULL)
	{
	}

    CDataReportImpl::~CDataReportImpl()
    {
    }

    STDMETHODIMP_(int32_t) CDataReportImpl::init(THIS_ GnBase* base)
	{      
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceDataReport, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CDataReportImpl::terminate(THIS_ )
	{
		GN_TRACE(kTraceInfo, kTraceDataReport, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceDataReport, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

	STDMETHODIMP_(int32_t) CDataReportImpl::test(THIS_)
	{
		// do something
		return 0;
	}

	STDMETHODIMP_(void) CDataReportImpl::onHttpTest(THIS_)
	{
		// do something
	}
}
