#ifndef _H_CFILEMODULE_H_
#define _H_CFILEMODULE_H_

#include "common_types.h"
#include "comdefs.h"
#include "modules/FileModule/interface/file_module.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib,"LogModule.dll.lib")

namespace gn
{
	class CFileModule : public IFileModule
	{
	public:
		CFileModule();
		virtual ~CFileModule();

	public:
		STDMETHOD_(int32_t,init) (GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_);
		STDMETHOD_(int32_t,test) (THIS_);

	private:
		GnBase*	m_gnBase;
	};
}
#endif // _H_CFILEMODULE_H_
