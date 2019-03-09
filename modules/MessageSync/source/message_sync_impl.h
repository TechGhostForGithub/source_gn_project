#ifndef _MESSAGE_SYNC_IMPL_H_
#define _MESSAGE_SYNC_IMPL_H_
#include "modules/MessageSync/interface/message_sync.h"

namespace gn
{
	class CMessageSyncImpl : public IMessageSync
	{
	public:
		CMessageSyncImpl();
		virtual ~CMessageSyncImpl();

	public:
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_);
		STDMETHOD_(int32_t,test) (THIS_);

	private:
		GnBase* m_gnBase;
	};

}
#endif // _MESSAGE_SYNC_IMPL_H_
