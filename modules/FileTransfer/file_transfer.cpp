#include "common_types.h"
#include "comdefs.h"
#include "modules/FileTransfer/source/file_transfer_impl.h"

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
		if (riid == IID_IFileTransfer) {
			CFileTransferImpl* p = new CUnknownObject<CFileTransferImpl>;
			(static_cast<IFileTransfer*>(p))->AddRef();

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
	if (CLSID_FileTransfer == rclsid) {
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


STDAPI FileTransfer_CreateObject(REFCLSID rclsid,
                                REFIID riid,
                                LPVOID* ppv)
{
    if (CLSID_FileTransfer == rclsid) {
		if (IID_IFileTransfer == riid) {
			CFileTransferImpl* p = new CUnknownObject<CFileTransferImpl>;
			(static_cast<IFileTransfer*>(p))->AddRef();

			*ppv = p;
            return 0;
        }
	}
	return -1;
}

#endif
