#ifndef _COMMON_HEAD_H_
#define _COMMON_HEAD_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {533162AC-1EE8-450b-A177-B7038E8846AD}
DEFINE_GUID(CLSID_CommonHead, 
			0x533162ac, 0x1ee8, 0x450b, 0xa1, 0x77, 0xb7, 0x3, 0x8e, 0x88, 0x46, 0xad);


#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI CommonHead_CreateObject(REFCLSID rclsid,
								REFIID riid,
								LPVOID* ppv);
#endif

// {AAF391DE-7203-4115-8619-0F183EBC1FAB}
DEFINE_GUID(IID_ICommonHead, 
			0xaaf391de, 0x7203, 0x4115, 0x86, 0x19, 0xf, 0x18, 0x3e, 0xbc, 0x1f, 0xab);


#ifdef __cplusplus
extern "C" {
#endif

	DECLARE_INTERFACE_(ICommonHeadObserver, IUnknown)
	{
		// hft参见common_types.h中HttpFailedType
		STDMETHOD_(void,onCommonHeadTest) () PURE;
	};

	DECLARE_INTERFACE_(ICommonHead, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_ ) PURE;
		STDMETHOD_(int32_t,registerCommonHeadObserver)(THIS_ ICommonHeadObserver* observer) PURE;
		STDMETHOD_(int32_t,deRegisterCommonHeadObserver)(THIS_ ICommonHeadObserver* observer) PURE;
		STDMETHOD_(int32_t,test)(THIS_ ) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _COMMON_HEAD_H_

