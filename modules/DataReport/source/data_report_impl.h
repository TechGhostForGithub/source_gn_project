#ifndef _DATA_REPORT_IMPL_H_
#define _DATA_REPORT_IMPL_H_
#include "common_types.h"
#include "comdefs.h"
#include "modules/DataReport/interface/data_report.h"
#include "modules/HttpKernel/interface/http_kernel.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib,"LogModule.dll.lib")

namespace gn
{
	class CDataReportImpl
		: public IDataReport
        , public CUnknownObject<IHttpKernelObserver>
	{
	public:
		CDataReportImpl();
		virtual ~CDataReportImpl();

		// IDataReport
        STDMETHOD_(int32_t,init) (GnBase* base);
        STDMETHOD_(int32_t,terminate) (THIS_);
        STDMETHOD_(int32_t,test) ();

        // IHttpKernelObserver
        STDMETHOD_(void, onHttpTest) (THIS_);

	private:	 
        GnBase* m_gnBase;
	};
}


#endif // _DATA_REPORT_IMPL_H_
