#ifndef _MESSAGE_SYNC_H_
#define _MESSAGE_SYNC_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {5D464E6A-498F-416b-A893-EF6A9E76E607}
DEFINE_GUID(CLSID_MessageSync, 
			0x5d464e6a, 0x498f, 0x416b, 0xa8, 0x93, 0xef, 0x6a, 0x9e, 0x76, 0xe6, 0x7);

// {AFFF9A0B-DCE3-4557-BA5F-A63BA4E75A85}
DEFINE_GUID(IID_IMessageSync, 
			0xafff9a0b, 0xdce3, 0x4557, 0xba, 0x5f, 0xa6, 0x3b, 0xa4, 0xe7, 0x5a, 0x85);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);

#else
STDAPI MessageSync_CreateObject(REFCLSID rclsid,
                                REFIID riid,
                                LPVOID* ppv);
#endif

#ifdef __cplusplus
extern "C" {
#endif

	DECLARE_INTERFACE_(IMessageSync, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_) PURE;
		STDMETHOD_(int32_t,test) (THIS_) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _MESSAGE_SYNC_H_
