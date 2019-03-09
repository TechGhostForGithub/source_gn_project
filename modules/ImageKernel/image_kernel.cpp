#include "common_types.h"
#include "comdefs.h"
#include "modules/ImageKernel/source/image_kernel_impl.h"

using namespace gn;
#ifdef _MSC_VER
struct CClassFactory : public IClassFactory
{
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(
		IUnknown *pUnkOuter,
		REFIID riid,
		void **ppvObject)
	{
		if (riid == IID_IImageKernel) {
			CImageKernelImpl* p = new CUnknownObject<CImageKernelImpl>;
			(static_cast<IImageKernel*>(p))->AddRef();

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
	if (CLSID_ImageKernel == rclsid) {
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

STDAPI ImageKernel_CreateObject(REFCLSID rclsid,
                                REFIID riid,
                                LPVOID* ppv)
{
    if (CLSID_ImageKernel == rclsid) {
		if (IID_IImageKernel == riid) {
			CImageKernelImpl* p = new CUnknownObject<CImageKernelImpl>;
			(static_cast<IImageKernel*>(p))->AddRef();

			*ppv = p;
            return 0;
        }
	}
	return -1;
}

#endif
