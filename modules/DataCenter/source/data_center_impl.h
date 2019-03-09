#ifndef _DATA_CENTER_IMPL_H_
#define _DATA_CENTER_IMPL_H_
#include "modules/DataCenter/interface/data_center.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib,"LogModule.dll.lib")

namespace gn
{
	class CDataCenterImpl : public IDataCenter
	{
	public:
		CDataCenterImpl();
		virtual ~CDataCenterImpl();

		// IDataCenter
		STDMETHOD_(int32_t,init)(THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_ );
		STDMETHOD_(int32_t,test) (THIS_);

	private:
		GnBase*	m_gnBase;
	};
}

#endif // _DATA_CENTER_IMPL_H_
