#ifndef _NETWORKSERVICEIMPL_H_
#define _NETWORKSERVICEIMPL_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/NetworkService/interface/network_service.h"

namespace gn
{
	class CNetworkServiceImpl : public INetworkService
	{
	public:
		CNetworkServiceImpl();
		virtual ~CNetworkServiceImpl();

	public:
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_);
		STDMETHOD_(int32_t,test) (THIS_);

	private:
		GnBase* m_gnBase;
	};

}
#endif // _NETWORKSERVICEIMPL_H_
