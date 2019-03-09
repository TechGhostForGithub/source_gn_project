#ifndef _DATA_REPORT_H_
#define _DATA_REPORT_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {6F829FC3-EFB3-4b74-866F-D545455BBF58}
DEFINE_GUID(CLSID_DataReport, 
			0x6f829fc3, 0xefb3, 0x4b74, 0x86, 0x6f, 0xd5, 0x45, 0x45, 0x5b, 0xbf, 0x58);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI DataReport_CreateObject(REFCLSID rclsid,
							 REFIID riid,
							 LPVOID* ppv);
#endif

// {C2276864-AAEF-45d4-ADCC-7787863C845F}
DEFINE_GUID(IID_IDataReport, 
			0xc2276864, 0xaaef, 0x45d4, 0xad, 0xcc, 0x77, 0x87, 0x86, 0x3c, 0x84, 0x5f);


#ifdef __cplusplus
extern "C" {
#endif
	DECLARE_INTERFACE_(IDataReport, IUnknown)
	{
		STDMETHOD_(int32_t,init) (gn::GnBase* base) PURE;
        STDMETHOD_(int32_t,terminate) (THIS_) PURE;
        STDMETHOD_(int32_t,test) () PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // _DATA_REPORT_H_
