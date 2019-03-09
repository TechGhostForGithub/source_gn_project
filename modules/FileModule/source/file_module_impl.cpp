
#include "modules/FileModule/source/file_module_impl.h"
#include <assert.h>

namespace gn
{
	CFileModule::CFileModule()
		: m_gnBase(NULL)
	{
	}

	CFileModule::~CFileModule()
	{
	}

	STDMETHODIMP_(int32_t) CFileModule::init(GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceFileModule, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CFileModule::terminate(THIS_)
	{
		GN_TRACE(kTraceInfo, kTraceFileModule, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceFileModule, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}

	STDMETHODIMP_(int32_t) CFileModule::test(THIS_)
	{
		// do something
		return 0;
	}
}
