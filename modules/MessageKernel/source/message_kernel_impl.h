#ifndef _MESSAGE_KERNEL_IMPL_H_
#define _MESSAGE_KERNEL_IMPL_H_
#include "modules/MessageKernel/interface/message_kernel.h"

namespace gn
{
	class CMessageKernel : public IMessageKernel
	{
	public:
		CMessageKernel();
		virtual ~CMessageKernel();

		// IMessageKernel
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_);
		STDMETHOD_(int32_t,test) (THIS_);

      private:
            GnBase* m_gnBase;
	};

}
#endif // _MESSAGE_KERNEL_IMPL_H_
