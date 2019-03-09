#ifndef _COMDEFS_H_
#define _COMDEFS_H_

#include "typedefs.h"

#if defined(WIN32)
#include <ObjBase.h>
#include <InitGuid.h>
#elif defined(CCORE_LINUX) || defined(CCORE_MAC) || defined(CCORE_ANDROID) || defined(CCORE_IOS)


typedef long HRESULT;
typedef void* LPVOID;


#define STDAPI extern "C" HRESULT

#define COM_DECLSPEC_NOTHROW
#define STDMETHODCALLTYPE
#define DECLSPEC_NOVTABLE

#define STDMETHODIMP_(type) type STDMETHODCALLTYPE
#define STDMETHOD(method) virtual COM_DECLSPEC_NOTHROW HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual COM_DECLSPEC_NOTHROW type STDMETHODCALLTYPE method
#define STDMETHODV(method) virtual COM_DECLSPEC_NOTHROW HRESULT STDMETHODVCALLTYPE method
#define STDMETHODV_(type,method) virtual COM_DECLSPEC_NOTHROW type STDMETHODVCALLTYPE method
#define PURE = 0
#define THIS_
#define THIS void
#define DECLARE_INTERFACE(iface) struct DECLSPEC_NOVTABLE iface
#define DECLARE_INTERFACE_(iface, baseiface) struct DECLSPEC_NOVTABLE iface : public baseiface
#define DECLARE_INTERFACE_IID(iface, iid) struct DECLSPEC_UUID(iid) DECLSPEC_NOVTABLE iface
#define DECLARE_INTERFACE_IID_(iface, baseiface, iid) struct DECLSPEC_UUID(iid) DECLSPEC_NOVTABLE iface : public baseiface

#if 0
	typedef struct _GUID {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	} GUID;

	typedef GUID IID;
	typedef GUID CLSID;

#	define REFIID const IID &
#	define REFCLSID const IID &

#	define DECLSPEC_SELECTANY
#	define FAR

#	define INITGUID
#	ifdef INITGUID
#	define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
		extern "C" const GUID DECLSPEC_SELECTANY name \
		= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#	else
#	define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
		extern "C" const GUID FAR name
#	endif // INITGUID
	inline int operator==(const GUID& guidOne, const GUID& guidOther)
	{
		return guidOne.Data1 == guidOther.Data1 &&
			guidOne.Data2 == guidOther.Data2 &&
			guidOne.Data3 == guidOther.Data3;
	}
#else
	typedef int GUID;
	typedef GUID IID;
	typedef GUID CLSID;
#	define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8)
#	define REFIID const IID &
#	define REFCLSID const IID &

#	define DECLSPEC_SELECTANY
#	define FAR

	enum
	{
		CLSID_DataCenter		= 0x0000, 
		IID_IDataCenter			= 0x0001,

		CLSID_DataReport		= 0x0010,
		IID_IDataReport			= 0x0011,

		CLSID_EngineModule		= 0x0020,
		IID_IEngineModule		= 0x0021,

		CLSID_FileModule		= 0x0030,
		IID_IFileModule			= 0x0031,

		CLSID_FileTransfer		= 0x0040,
		IID_IFileTransfer		= 0x0041,

		CLSID_ImageKernel		= 0x0050,
		IID_IImageKernel		= 0x0051,

		CLSID_MessageSync		= 0x0060,
		IID_IMessageSync		= 0x0061,

		CLSID_NetworkService	= 0x0070,
		IID_INetworkService		= 0x0071,

		CLSID_UserFace			= 0x0080,
		IID_IUserFace			= 0x0081,

		CLSID_MessageKernel		= 0x0090,
		IID_IMessageKernel		= 0x0091,
		IID_IMessageBuddy		= 0x0092,
		IID_IMessageLogin		= 0x0095,
		IID_IMessageMaskWord	= 0x0096,
		IID_IMessageMis			= 0x0097,
		IID_IMessageNetseting	= 0x0098,
		IID_IMessageNotice		= 0x0099,
		IID_IMessageUserInfo	= 0x009D,
		IID_IMessageMiscellaneous	= 0x009F,
		IID_IMessageTgroup		= 0x00A0,
		IID_IMessageConsole		= 0x00A1,
        IID_IMessageOfficialAccount	= 0x00A2,
        IID_IMessageSearch      = 0x00A3,
        IID_IMessageWidget      = 0x00A4,
        IID_IMessagePassAPI     = 0x00A5,

		CLSID_NetworkRouting	= 0x00B0,
		IID_INetworkRouting		= 0X00B1,

		CLSID_CommonHead		= 0x00C0,
		IID_ICommonHead			= 0x00C1,

        CLSID_HttpKernel        = 0x00D0,
        IID_IHttpKernel         = 0x00D1,
        
        CLSID_H5OfflinePkg      = 0x00E0,
        IID_IH5OfflinePkg       = 0x00E1,
	};
#endif


//-----------------------------------------------------------------------------
#if defined(CCORE_MAC) || defined(CCORE_IOS)
#include <libkern/OSAtomic.h>
#include <stdlib.h>
#elif defined(CCORE_LINUX) || defined(CCORE_ANDROID)
#include <inttypes.h>
#include <malloc.h>
#endif
struct IUnknown
{
	virtual long STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) = 0;
	virtual unsigned long STDMETHODCALLTYPE AddRef(void) = 0;
	virtual unsigned long STDMETHODCALLTYPE Release(void) = 0;
};

#endif

template <typename T> class CUnknownObject : public T
{
public:
	CUnknownObject()
	{ 
		m_nRefCount = 0;
	}

	virtual ~CUnknownObject()
	{
		m_nRefCount = 0;
	}
	virtual long STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject)
	{
		*ppvObject = (T *)this;
		this->AddRef();
		return 0;
	}

	virtual unsigned long STDMETHODCALLTYPE AddRef(void)
	{
#if defined(WIN32)
		return static_cast<unsigned long>(InterlockedIncrement(
			reinterpret_cast<volatile long*>(&m_nRefCount)));
#elif defined(CCORE_MAC) || defined(CCORE_IOS)
		return OSAtomicIncrement32Barrier(&m_nRefCount);
#elif defined(CCORE_LINUX) || defined(CCORE_ANDROID)
		return __sync_fetch_and_add(&m_nRefCount, 1) + 1;
#else
		int ref = ++m_nRefCount;

		return static_cast<unsigned long>(ref);
#endif
	}

	virtual unsigned long STDMETHODCALLTYPE Release(void)
	{
#if defined(WIN32)
		long ref = static_cast<long>(InterlockedDecrement(
			reinterpret_cast<volatile long*>(&m_nRefCount)));
#elif defined(CCORE_MAC) || defined(CCORE_IOS)
		long ref = OSAtomicDecrement32Barrier(&m_nRefCount);
#elif defined(CCORE_LINUX) || defined(CCORE_ANDROID)
		long ref = __sync_fetch_and_sub(&m_nRefCount, 1) - 1;
#else
		long ref = --m_nRefCount;
#endif

		if (ref == 0)
		{
			delete this;
		}
		return static_cast<unsigned long>(ref);
	}

	long GetRef()
	{
		return m_nRefCount;
	}

private:
	int m_nRefCount;
};

#endif // _COMDEFS_H_

//#define UI_CCTALK_OEM_WANGXIAO
