#ifndef _IMAGE_KERNEL_H_
#define _IMAGE_KERNEL_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {C0899132-79E5-4885-8DDD-ABFA8967396B}
DEFINE_GUID(CLSID_ImageKernel, 
			0xc0899132, 0x79e5, 0x4885, 0x8d, 0xdd, 0xab, 0xfa, 0x89, 0x67, 0x39, 0x6b);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI ImageKernel_CreateObject(REFCLSID rclsid,
                                  REFIID riid,
                                  LPVOID* ppv);
#endif

// {D45320BA-EC1C-464b-B015-7F06B72729B1}
DEFINE_GUID(IID_IImageKernel, 
			0xd45320ba, 0xec1c, 0x464b, 0xb0, 0x15, 0x7f, 0x6, 0xb7, 0x27, 0x29, 0xb1);

#ifdef __cplusplus
extern "C" {
#endif

	DECLARE_INTERFACE_(IImageKernel, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_ ) PURE;
		STDMETHOD_(int32_t,test) (THIS_) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _IMAGE_KERNEL_H_
