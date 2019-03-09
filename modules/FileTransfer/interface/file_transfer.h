#ifndef _FILE_TRANSFER_H_
#define _FILE_TRANSFER_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {BF4F9CE8-224F-4525-827A-F253B98AE9CB}
DEFINE_GUID(CLSID_FileTransfer, 
			0xbf4f9ce8, 0x224f, 0x4525, 0x82, 0x7a, 0xf2, 0x53, 0xb9, 0x8a, 0xe9, 0xcb);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI FileTransfer_CreateObject(REFCLSID rclsid,
							   REFIID riid,
							   LPVOID* ppv);
#endif

// {51F5B18B-3C8F-4359-A868-E6F683977E77}
DEFINE_GUID(IID_IFileTransfer, 
			0x51f5b18b, 0x3c8f, 0x4359, 0xa8, 0x68, 0xe6, 0xf6, 0x83, 0x97, 0x7e, 0x77);

#ifdef __cplusplus
extern "C" {
#endif
	DECLARE_INTERFACE_(IFileTransfer, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_) PURE;
		STDMETHOD_(int32_t,test) (THIS_) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _FILE_TRANSFER_H_
