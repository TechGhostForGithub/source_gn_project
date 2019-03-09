// UserFace.cpp : 定义 DLL 应用程序的导出函数。
//

#include "common_types.h"
#include "comdefs.h"
#include "modules/UserFace/source/user_face_impl.h"

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
		if (riid == IID_IUserFace) {
			CUserFaceImpl* p = new CUnknownObject<CUserFaceImpl>;
			(static_cast<IUserFace*>(p))->AddRef();

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
	if (CLSID_UserFace == rclsid) {
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


STDAPI UserFace_CreateObject(REFCLSID rclsid,
                                REFIID riid,
                                LPVOID* ppv)
{
    if (CLSID_UserFace == rclsid) {
		if (IID_IUserFace == riid) {
			CUserFaceImpl* p = new CUnknownObject<CUserFaceImpl>;
			(static_cast<IUserFace*>(p))->AddRef();

			*ppv = p;
            return 0;
        }
	}
	return -1;
}

#endif
