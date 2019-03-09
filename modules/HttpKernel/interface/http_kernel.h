#ifndef _HTTP_KERNEL_H_
#define _HTTP_KERNEL_H_

#include "modules/ClientKernel/interface/gn_base.h"

// {8F948690-BDB3-4e48-895E-87190DF3E6BD}
DEFINE_GUID(CLSID_HttpKernel, 
            0x8f948690, 0xbdb3, 0x4e48, 0x89, 0x5e, 0x87, 0x19, 0xd, 0xf3, 0xe6, 0xbd);

// {D8B70BA4-9A6C-4e6f-9E50-D6ABE757E903}
DEFINE_GUID(IID_IHttpKernel, 
            0xd8b70ba4, 0x9a6c, 0x4e6f, 0x9e, 0x50, 0xd6, 0xab, 0xe7, 0x57, 0xe9, 0x3);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI HttpKernel_CreateObject(REFCLSID rclsid,
                               REFIID riid,
                               LPVOID* ppv);
#endif

#ifdef __cplusplus
extern "C" {
#endif
    DECLARE_INTERFACE_(IHttpKernelObserver, IUnknown)
    {
        STDMETHOD_(void, onHttpTest) (THIS_) PURE;
    };

	DECLARE_INTERFACE_(IHttpKernel, IUnknown)
	{
		STDMETHOD_(int32_t, init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t, terminate) (THIS_) PURE;

        STDMETHOD_(int32_t, registerHttpKernelObserver) (THIS_ IHttpKernelObserver* observer) PURE;
        STDMETHOD_(int32_t, deRegisterHttpKernelObserver) (THIS_ IHttpKernelObserver* observer) PURE;

        STDMETHOD_(int32_t, test) (THIS_) PURE;
	};

#ifdef __cplusplus
}
#endif
#endif // _HTTP_KERNEL_H_
