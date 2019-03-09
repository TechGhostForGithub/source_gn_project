// DataCenter.cpp : 定义 DLL 应用程序的导出函数。
//
#include "common_types.h"
#include "comdefs.h"
#include "modules/DataCenter/source/data_center_impl.h"

using namespace gn;

#ifdef _MSC_VER
#include <atlcomcli.h>
struct CClassFactory : public IClassFactory
{
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(
		IUnknown *pUnkOuter,
		REFIID riid,
		void **ppvObject) {
		if (riid == IID_IDataCenter) {
			CDataCenterImpl* p = new CUnknownObject<CDataCenterImpl>;
			(static_cast<IDataCenter*>(p))->AddRef();

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
	if (CLSID_DataCenter == rclsid) {
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

STDAPI DataCenter_CreateObject(REFCLSID rclsid,
						       REFIID riid,
                               LPVOID* ppv)
{
    if (CLSID_DataCenter == rclsid){
		if (IID_IDataCenter == riid) {
			gn::CDataCenterImpl* p = new CUnknownObject<gn::CDataCenterImpl>;
			(static_cast<IDataCenter*>(p))->AddRef();

			*ppv = p;
            return 0;
        }
	}
	return -1;
}

#endif
