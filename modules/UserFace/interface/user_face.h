#ifndef _USER_FACE_H_
#define _USER_FACE_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {4EA93DB6-4832-4281-AD5C-E74B0160669D}
DEFINE_GUID(CLSID_UserFace, 
			0x4ea93db6, 0x4832, 0x4281, 0xad, 0x5c, 0xe7, 0x4b, 0x1, 0x60, 0x66, 0x9d);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI UserFace_CreateObject(REFCLSID rclsid,
								 REFIID riid,
								 LPVOID* ppv);
#endif

// {3E979B0A-B9DB-4c5f-8A17-A34800DAE129}
DEFINE_GUID(IID_IUserFace, 
			0x3e979b0a, 0xb9db, 0x4c5f, 0x8a, 0x17, 0xa3, 0x48, 0x0, 0xda, 0xe1, 0x29);

#ifdef __cplusplus
extern "C" {
#endif
	DECLARE_INTERFACE_(IUserFace, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_ ) PURE;
		STDMETHOD_(int32_t,test) (THIS_ ) PURE;
	};

#ifdef __cplusplus
}
#endif
#endif // _USER_FACE_H_
