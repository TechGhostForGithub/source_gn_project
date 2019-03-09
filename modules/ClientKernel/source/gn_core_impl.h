#ifndef _GN_CORE_IMPL_H_
#define _GN_CORE_IMPL_H_

#include <assert.h>
#include "system_wrappers/interface/atomic32.h"
#include "modules/ClientKernel/interface/gn_base.h"
#include "modules/ClientKernel/source/gn_shared_data.h"
#include "modules/ClientKernel/source/gn_base_impl.h"
#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif
namespace gn
{
	class GnCoreImpl
		: public KernelSharedData  // Must be the first base class
		, public GnCore
		, public GnBaseImpl
	{
	public:
		GnCoreImpl(void);
		virtual ~GnCoreImpl();

		int32_t addRef();

		// This implements the Release() method for all the inherited interfaces.
		virtual int32_t release();

	private:
		Atomic32 m_ref_count;
	};
}
#ifdef _MSC_VER
#pragma warning(default:4355)
#endif

#endif // _GN_CORE_IMPL_H_
