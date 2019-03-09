#ifndef _NETWORK_ROUTING_H_
#define _NETWORK_ROUTING_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {CF1D05E4-7684-48e5-927D-B9771A317736}
DEFINE_GUID(CLSID_NetworkRouting, 
			0xcf1d05e4, 0x7684, 0x48e5, 0x92, 0x7d, 0xb9, 0x77, 0x1a, 0x31, 0x77, 0x36);

// {D27B0F49-887F-4aa3-9B27-E0CC9A429B90}
DEFINE_GUID(IID_INetworkRouting, 
			0xd27b0f49, 0x887f, 0x4aa3, 0x9b, 0x27, 0xe0, 0xcc, 0x9a, 0x42, 0x9b, 0x90);

#ifdef _MSC_VER

STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);

#else

STDAPI NetworkRouting_CreateObject(REFCLSID rclsid,REFIID riid,LPVOID* ppv);

#endif

#ifdef __cplusplus
extern "C" {
#endif

	DECLARE_INTERFACE_(INetworkRouting, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_) PURE;
		STDMETHOD_(int32_t,test) (THIS_ ) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _NETWORK_ROUTING_H_
