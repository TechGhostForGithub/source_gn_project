#ifndef _COMMON_HEAD_IMPL_H_
#define _COMMON_HEAD_IMPL_H_
#include <map>
#include <set>
#include <string>

#include "modules/HttpKernel/interface/http_kernel.h"
#include "modules/CommonHead/interface/common_head.h"
//#pragma comment(lib,"system_wrappers.lib")
//#pragma comment(lib,"ws2_32.lib")

namespace gn
{
	class CCommonHeadImpl
		: public ICommonHead
		, public CUnknownObject<IHttpKernelObserver>
	{
	public:
		CCommonHeadImpl();
		virtual ~CCommonHeadImpl();

		// ICommonHead
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_ );
		STDMETHOD_(int32_t,registerCommonHeadObserver)(THIS_ ICommonHeadObserver* observer);
		STDMETHOD_(int32_t,deRegisterCommonHeadObserver)(THIS_ ICommonHeadObserver* observer);
		STDMETHOD_(int32_t,test)(THIS_);

        // IHttpKernelObserver
        STDMETHOD_(void, onHttpTest) (THIS_);

	private:
		GnBase*	m_gnBase;
	};
}


#endif // _COMMON_HEAD_IMPL_H_
