#ifndef _HTTP_KERNEL_IMPL_H_
#define _HTTP_KERNEL_IMPL_H_
#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"
#include "modules/HttpKernel/interface/http_kernel.h"

namespace gn
{
	class CHttpKernel : public IHttpKernel
	{
	public:
		CHttpKernel();
		virtual ~CHttpKernel();

    public:
		// IHttpKernel
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_);

        STDMETHOD_(int32_t, registerHttpKernelObserver) (THIS_ IHttpKernelObserver* observer);
        STDMETHOD_(int32_t, deRegisterHttpKernelObserver) (THIS_ IHttpKernelObserver* observer);

        STDMETHOD_(int32_t, test) (THIS_);

    private:
        GnBase* m_gnBase;
	};

}
#endif // _HTTP_KERNEL_IMPL_H_
