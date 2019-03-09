// MessageKernel.cpp : 定义 DLL 应用程序的导出函数。
//



#include "common_types.h"
#include "comdefs.h"
#include "modules/NetworkService/source/network_service_impl.h"

using namespace gn;

#ifdef _MSC_VER
#include <atlcomcli.h>
struct CClassFactory : public IClassFactory
{
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(
		IUnknown *pUnkOuter,
		REFIID riid,
		void **ppvObject)
	{
		if (riid == IID_INetworkService) {
			CNetworkServiceImpl* p = new CUnknownObject<CNetworkServiceImpl>;
			p->AddRef();

			*ppvObject = p;

			return S_OK;
		}
		return E_NOTIMPL;
	}

	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock)
	{
		return E_NOTIMPL;
	}
};

STDAPI DllGetClassObject(REFCLSID rclsid,
						 REFIID riid,
						 LPVOID* ppv)
{
	if (CLSID_NetworkService == rclsid) {
		if (IID_IClassFactory == riid) {
			CClassFactory *pf = new CUnknownObject<CClassFactory>;
			pf->AddRef();

			*ppv = pf;
			return S_OK;
		}
	}
	return E_NOTIMPL;
}

#else

STDAPI NetworkService_CreateObject(REFCLSID rclsid,
                                   REFIID riid,
                                   LPVOID* ppv)
{
    if (CLSID_NetworkService == rclsid) {
		if (IID_INetworkService == riid) {
			CNetworkServiceImpl* p = new CUnknownObject<CNetworkServiceImpl>;
			p->AddRef();

			*ppv = p;

            return 0;
        }
	}
	return -1;
}

#endif
