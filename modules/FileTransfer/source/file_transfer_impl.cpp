#include "modules/FileTransfer/source/file_transfer_impl.h"
#include <assert.h>

namespace gn
{
	CFileTransferImpl::CFileTransferImpl()
		: m_gnBase(NULL)
	{
	}

	CFileTransferImpl::~CFileTransferImpl()
	{
	}

	STDMETHODIMP_(int32_t) CFileTransferImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceFileTransfer, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CFileTransferImpl::terminate(THIS_)
	{
		GN_TRACE(kTraceInfo, kTraceFileTransfer, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceFileTransfer, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;      

		return 0;
	}
    
    STDMETHODIMP_(int32_t) CFileTransferImpl::test(THIS_)
    {
        // do something
        return 0;
    }
}