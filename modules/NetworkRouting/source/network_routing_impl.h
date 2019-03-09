#ifndef _NETWORK_ROUTING_IMPL_H_
#define _NETWORK_ROUTING_IMPL_H_

#include "modules/NetworkRouting/interface/network_routing.h"

namespace gn
{
	class CNetworkRoutingImpl : public INetworkRouting
	{
	public:
		CNetworkRoutingImpl();
		virtual ~CNetworkRoutingImpl();

	public:
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_);
		STDMETHOD_(int32_t,test) (THIS_);

	private:
		GnBase* m_gnBase;
	};
}  // namespace gn

#endif // _NETWORK_ROUTING_IMPL_H_
