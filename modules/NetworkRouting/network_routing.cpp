#include "modules/NetworkRouting/source/network_routing_impl.h"
using namespace gn;

#ifdef _WIN32
#include <atlcomcli.h>
struct CClassFactory : public IClassFactory
{
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(
		IUnknown *pUnkOuter,
		REFIID riid,
		void **ppvObject)
	{
		if (riid == IID_INetworkRouting) {
			CNetworkRoutingImpl* p = new CUnknownObject<CNetworkRoutingImpl>;
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
	if (CLSID_NetworkRouting == rclsid) {
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

STDAPI NetworkRouting_CreateObject(REFCLSID rclsid,
                                REFIID riid,
                                LPVOID* ppv)
{
    if (CLSID_NetworkRouting == rclsid) {
		if (IID_INetworkRouting == riid) {
            CNetworkRoutingImpl* p = new CUnknownObject<CNetworkRoutingImpl>;
            (static_cast<INetworkRouting*>(p))->AddRef();
            
            *ppv = p;
            
            return 0;
        }
	}
	return -1;
}

#endif
