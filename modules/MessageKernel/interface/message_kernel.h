#ifndef _MESSAGE_KERNEL_H_
#define _MESSAGE_KERNEL_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/ClientKernel/interface/gn_base.h"

// {E0D1CC64-29FD-4277-9D94-FEC6D6FCA1C3}
DEFINE_GUID(CLSID_MessageKernel, 
			0xe0d1cc64, 0x29fd, 0x4277, 0x9d, 0x94, 0xfe, 0xc6, 0xd6, 0xfc, 0xa1, 0xc3);

// {446601CF-0759-4022-A719-E01FE7998E4E}
DEFINE_GUID(IID_IMessageKernel, 
			0x446601cf, 0x759, 0x4022, 0xa7, 0x19, 0xe0, 0x1f, 0xe7, 0x99, 0x8e, 0x4e);

#ifdef _MSC_VER
STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv);
#else
STDAPI MessageKernel_CreateObject(REFCLSID rclsid,
                                  REFIID riid,
                                  LPVOID* ppv);
#endif

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_INTERFACE_(IMessageKernel, IUnknown)
{
	STDMETHOD_(int32_t,init) (THIS_ gn::GnBase* base) PURE;
	STDMETHOD_(int32_t,terminate) (THIS_) PURE;
	STDMETHOD_(int32_t,test) (THIS_) PURE;
};

#ifdef __cplusplus
}
#endif
#endif // _MESSAGE_KERNEL_H_
