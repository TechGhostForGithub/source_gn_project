#ifndef _ENGINE_MODULE_H_
#define _ENGINE_MODULE_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {0683FF8E-1D65-46e8-98E1-0ADF46E015AF}
DEFINE_GUID(CLSID_EngineModule, 
			0x683ff8e, 0x1d65, 0x46e8, 0x98, 0xe1, 0xa, 0xdf, 0x46, 0xe0, 0x15, 0xaf);

// {B5333E5B-D0D1-4879-8DAF-461065B738A1}
DEFINE_GUID(IID_IEngineModule, 
			0xb5333e5b, 0xd0d1, 0x4879, 0x8d, 0xaf, 0x46, 0x10, 0x65, 0xb7, 0x38, 0xa1);


#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI EngineModule_CreateObject(REFCLSID rclsid,
							   REFIID riid,
							   LPVOID* ppv);
#endif

#ifdef __cplusplus
extern "C" {
#endif

	DECLARE_INTERFACE_(IEngineModule, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_) PURE;
		STDMETHOD_(int32_t,test) (THIS_) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _ENGINE_MODULE_H_
