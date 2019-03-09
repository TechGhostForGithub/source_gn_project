#include <assert.h>
#include "modules/ClientKernel/source/gn_shared_data.h"
#include "modules/LogModule/log_module.h"

namespace gn
{
	KernelSharedData::KernelSharedData()
		: m_gnCore(NULL)
	{
		GnLog::createTrace();
	}

	KernelSharedData::~KernelSharedData()
	{
		assert(NULL == m_gnCore);
	}

	GnCore* KernelSharedData::Core()
	{
		assert(m_gnCore);
		return m_gnCore;
	}

}  // namespace gn
