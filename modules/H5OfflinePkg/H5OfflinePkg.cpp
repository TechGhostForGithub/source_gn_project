#include "common_types.h"
#include "comdefs.h"
#include "modules/H5OfflinePkg/source/h5_offline_pkg_impl.h"

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
        if (riid == IID_IH5OfflinePkg) {
            CH5OfflinePkgImpl* p = new CUnknownObject<CH5OfflinePkgImpl>;
            (static_cast<IH5OfflinePkg*>(p))->AddRef();

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
    if (CLSID_H5OfflinePkg == rclsid) {
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


STDAPI H5OfflinePkg_CreateObject(REFCLSID rclsid,
                                 REFIID riid,
                                 LPVOID* ppv)
{
    if (CLSID_H5OfflinePkg == rclsid) {
        if (IID_IH5OfflinePkg == riid) {
            CH5OfflinePkgImpl* p = new CUnknownObject<CH5OfflinePkgImpl>;
            (static_cast<IH5OfflinePkg*>(p))->AddRef();

            *ppv = p;
            return 0;
        }
    }
    return -1;
}

#endif
