#ifndef _IMAGE_KERNEL_IMPL_H_
#define _IMAGE_KERNEL_IMPL_H_
#include "modules/ImageKernel/interface/image_kernel.h"

namespace gn
{
	class CImageKernelImpl : public IImageKernel
	{
	public:
		CImageKernelImpl();
		virtual ~CImageKernelImpl();

		// IImageKernel
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_ );
		STDMETHOD_(int32_t,test) (THIS_);

	private:
        GnBase*	m_gnBase;
	};
}

#endif // _IMAGE_KERNEL_IMPL_H_
