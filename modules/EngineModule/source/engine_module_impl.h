#ifndef _ENGINE_MODULE_IMPL_H_
#define _ENGINE_MODULE_IMPL_H_
#include "common_types.h"
#include "comdefs.h"
#include "modules/EngineModule/interface/engine_module.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib,"LogModule.dll.lib")

namespace gn
{
	class CEngineModuleImpl : public IEngineModule
	{
	public:
		CEngineModuleImpl();
		virtual ~CEngineModuleImpl();

	public:
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_);

		STDMETHOD_(int32_t,test) (THIS_);

	private:
		GnBase*	m_gnBase;
	};
}

#endif // _ENGINE_MODULE_IMPL_H_
