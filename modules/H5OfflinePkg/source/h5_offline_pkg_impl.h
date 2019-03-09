#ifndef _H5_OFFLINE_PKG_IMPL_H_
#define _H5_OFFLINE_PKG_IMPL_H_
#include "common_types.h"
#include "modules/H5OfflinePkg/interface/h5_offline_pkg.h"

namespace gn
{
	class CH5OfflinePkgImpl : public IH5OfflinePkg
	{
	public:
		CH5OfflinePkgImpl();
		virtual ~CH5OfflinePkgImpl();

    public:
		// IH5OfflinePkg
        STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_ );
        STDMETHOD_(int32_t, test) (THIS_);

	private:
        GnBase* m_gnBase;
	};
}


#endif // _H5_OFFLINE_PKG_IMPL_H_
