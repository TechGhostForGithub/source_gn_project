#ifndef _INETWORKSERVICE_H_
#define _INETWORKSERVICE_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {20994F3B-CCA2-41a2-9F44-DCF60B78D3C4}
DEFINE_GUID(CLSID_NetworkService, 
			0x20994f3b, 0xcca2, 0x41a2, 0x9f, 0x44, 0xdc, 0xf6, 0xb, 0x78, 0xd3, 0xc4);


// {517807F4-A60F-4aca-AB77-A40331E78DF2}
DEFINE_GUID(IID_INetworkService, 
			0x517807f4, 0xa60f, 0x4aca, 0xab, 0x77, 0xa4, 0x3, 0x31, 0xe7, 0x8d, 0xf2);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI NetworkService_CreateObject(REFCLSID rclsid,
                                   REFIID riid,
                                   LPVOID* ppv);
#endif

#ifdef __cplusplus
extern "C" {
#endif

	DECLARE_INTERFACE_(INetworkService, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_) PURE;
		STDMETHOD_(int32_t,test) (THIS_) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _INETWORKSERVICE_H_
