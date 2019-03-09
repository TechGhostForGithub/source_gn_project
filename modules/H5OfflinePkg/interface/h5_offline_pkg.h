#ifndef _H5_OFFLINE_PKG_H_
#define _H5_OFFLINE_PKG_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {1AEC0861-8CD0-4b59-A34B-F5010B7AEF2B}
DEFINE_GUID(CLSID_H5OfflinePkg, 
            0x1aec0861, 0x8cd0, 0x4b59, 0xa3, 0x4b, 0xf5, 0x1, 0xb, 0x7a, 0xef, 0x2b);


#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);

#else
STDAPI H5OfflinePkg_CreateObject(REFCLSID rclsid,
							   REFIID riid,
							   LPVOID* ppv);
#endif

// {94753AFC-5AAF-4e57-A271-B41FA4330F26}
DEFINE_GUID(IID_IH5OfflinePkg, 
            0x94753afc, 0x5aaf, 0x4e57, 0xa2, 0x71, 0xb4, 0x1f, 0xa4, 0x33, 0xf, 0x26);

#ifdef __cplusplus
extern "C" {
#endif
	DECLARE_INTERFACE_(IH5OfflinePkg, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_ ) PURE;
        STDMETHOD_(int32_t, test) (THIS_) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _H5_OFFLINE_PKG_H_
