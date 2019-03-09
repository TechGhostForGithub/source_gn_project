#ifndef _DATA_CENTER_H_
#define _DATA_CENTER_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {16E20DE3-F791-42f4-B2A0-F295C96A4C7C}
DEFINE_GUID(CLSID_DataCenter, 
			0x16e20de3, 0xf791, 0x42f4, 0xb2, 0xa0, 0xf2, 0x95, 0xc9, 0x6a, 0x4c, 0x7c);

// {B4E88B82-7104-4eec-BDDE-DFBF8FD7BDE3}
DEFINE_GUID(IID_IDataCenter, 
			0xb4e88b82, 0x7104, 0x4eec, 0xbd, 0xde, 0xdf, 0xbf, 0x8f, 0xd7, 0xbd, 0xe3);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI DataCenter_CreateObject(REFCLSID rclsid,
						       REFIID riid,
                               LPVOID* ppv);

#endif

#ifdef __cplusplus
extern "C" {
#endif

	DECLARE_INTERFACE_(IDataCenter, IUnknown)
	{
		STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* pBase) PURE;
		STDMETHOD_(int32_t,terminate) (THIS_ ) PURE;
		STDMETHOD_(int32_t,test) (THIS_) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _DATA_CENTER_H_
