﻿#include "modules/HttpKernel/source/http_kernel_impl.h"

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
        if (riid == IID_IHttpKernel) {
            CHttpKernel* p = new CUnknownObject<CHttpKernel>;
            (static_cast<IHttpKernel*>(p))->AddRef();

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
    if (CLSID_HttpKernel == rclsid) {
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

STDAPI HttpKernel_CreateObject(REFCLSID rclsid,
                                  REFIID riid,
                                  LPVOID* ppvObject)
{
    if (CLSID_HttpKernel == rclsid) {
        if (riid == IID_IHttpKernel) {
            CHttpKernel* p = new CUnknownObject<CHttpKernel>;
            (static_cast<IHttpKernel*>(p))->AddRef();

            *ppvObject = p;

            return 0;
        }
    }
    return -1;
}

#endif