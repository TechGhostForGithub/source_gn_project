
#include "modules/UserFace/source/user_face_impl.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib, "LogModule.dll.lib")

namespace gn
{
	CUserFaceImpl::CUserFaceImpl()
		: m_gnBase(NULL)
	{
	}

	CUserFaceImpl::~CUserFaceImpl()
	{
	}

	STDMETHODIMP_(int32_t) CUserFaceImpl::init(THIS_ GnBase* base)
	{
		if (m_gnBase) {
			GN_TRACE(kTraceError, kTraceUserFace, 0, "%s gnBase object already enabled", __FUNCTION__);
			return -1;
		}

		m_gnBase = base;

		return 0;
	}

	STDMETHODIMP_(int32_t) CUserFaceImpl::terminate(THIS_)
	{
		GN_TRACE(kTraceInfo, kTraceUserFace, 0,"%s",__FUNCTION__);
        if (NULL == m_gnBase) {
            GN_TRACE(kTraceWarning, kTraceUserFace, 0, "%s gnBase object already disabled",__FUNCTION__);
            return -1;
        }

        m_gnBase = NULL;

		return 0;
	}

	STDMETHODIMP_(int32_t) CUserFaceImpl::test(THIS_)
	{
		// do something
		return 0;
	}
}

