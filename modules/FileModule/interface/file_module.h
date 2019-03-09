#ifndef _FILE_MODULE_H_
#define _FILE_MODULE_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {1BC97736-C621-44ec-B2E9-9D402E253614}
DEFINE_GUID(CLSID_FileModule, 
			0x1bc97736, 0xc621, 0x44ec, 0xb2, 0xe9, 0x9d, 0x40, 0x2e, 0x25, 0x36, 0x14);
#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI FileModule_CreateObject(REFCLSID rclsid,
								 REFIID riid,
                               LPVOID* ppv);

#endif
// {D0763DEE-27FB-49a1-913E-B71877DC416A}
DEFINE_GUID(IID_IFileModule, 
			0xd0763dee, 0x27fb, 0x49a1, 0x91, 0x3e, 0xb7, 0x18, 0x77, 0xdc, 0x41, 0x6a);

DECLARE_INTERFACE_(IFileModule, IUnknown)
{
	STDMETHOD_(int32_t,init) (gn::GnBase* base) PURE;
	STDMETHOD_(int32_t,terminate) (THIS_ ) PURE;
	STDMETHOD_(int32_t,test) (THIS_) PURE;
};

#endif // _FILE_MODULE_H_
