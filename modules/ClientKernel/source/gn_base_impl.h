#ifndef _GN_BASE_IMPL_H_
#define _GN_BASE_IMPL_H_

#include "modules/ClientKernel/interface/gn_base.h"

#ifdef _MSC_VER
#include "modules/ClientKernel/source/gn_modules_win.h"
#else
#include "modules/ClientKernel/source/gn_modules_posix.h"
#endif

namespace gn
{
	class KernelSharedData;
	class GnBaseImpl
		: public GnBase
	{
	public:
		GnBaseImpl(KernelSharedData* shared);

		virtual ~GnBaseImpl();

		virtual int32_t init();

		virtual int32_t terminate();

		virtual int32_t queryInterface(REFCLSID rclsid,REFIID riid,void** ppv);

	private:
		KernelSharedData*		m_sharedData;
		GnModules				m_modules;
	};
}  // namespace gn

#endif // _HJ_BASE_IMPL_H_
