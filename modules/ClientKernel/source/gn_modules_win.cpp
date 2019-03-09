#include "modules/ClientKernel/source/gn_modules_win.h"
#include "modules/MessageKernel/interface/message_kernel.h"
#include "modules/MessageSync/interface/message_sync.h"
#include "modules/NetworkService/interface/network_service.h"
#include "modules/FileModule/interface/file_module.h"
#include "modules/UserFace/interface/user_face.h"
#include "modules/DataCenter/interface/data_center.h"
#include "modules/DataReport/interface/data_report.h"
#include "modules/ImageKernel/interface/image_kernel.h"
#include "modules/CommonHead/interface/common_head.h"
#include "modules/EngineModule/interface/engine_module.h"
#include "modules/NetworkRouting/interface/network_routing.h"
#include "modules/FileTransfer/interface/file_transfer.h"
#include "system_wrappers/interface/utf_util_win.h"
#include "modules/LogModule/log_module.h"
#include "modules/HttpKernel/interface/http_kernel.h"
#include "modules/H5OfflinePkg/interface/h5_offline_pkg.h"
#include <assert.h>

namespace gn
{
#ifndef LOAD_DLL_FROM_XML
	ComItem GnModules::m_comDatabase[] = 
	{
		{_T("MessageKernel.dll"),	CLSID_MessageKernel,	NULL},
		{_T("MessageSync.dll"),		CLSID_MessageSync,		NULL},
		{_T("NetworkService.dll"),	CLSID_NetworkService,	NULL},
		{_T("FileModule.dll"),		CLSID_FileModule,		NULL},
		{_T("UserFace.dll"),		CLSID_UserFace,			NULL},
		{_T("DataCenter.dll"),		CLSID_DataCenter,		NULL},
        {_T("DataReport.dll"),      CLSID_DataReport,       NULL},
		{_T("ImageKernel.dll"),		CLSID_ImageKernel,		NULL},
		{_T("EngineModule.dll"),	CLSID_EngineModule,		NULL},
		{_T("NetworkRouting.dll"),	CLSID_NetworkRouting,	NULL},
		{_T("FileTransfer.dll"),	CLSID_FileTransfer,		NULL},
		{_T("CommonHead.dll"),		CLSID_CommonHead,		NULL},
        {_T("HttpKernel.dll"),      CLSID_HttpKernel,       NULL},
        {_T("H5OfflinePkg.dll"),    CLSID_H5OfflinePkg,     NULL},
	};
#endif

	GnModules::GnModules()
	{
	}

	GnModules::~GnModules()
	{
		assert(m_coms.size() == 0);
	}

	int32_t GnModules::loadResource()
	{
		return 0;
	}

	int32_t GnModules::unloadResource()
	{
		if (m_coms.size() == 0)
		{
			GN_TRACE(kTraceWarning, kTraceClientKernel, 0, "%s resource size is 0",__FUNCTION__);
			return -1;
		}
		
        // 第一次遍历：释放那些未被其他对象包含的对象
        // 第二次遍历：释放那些未释放的对象, 并且卸载相应的模块
        // 之所以两次循环，是为了处理相互包含的情况
		Coms::iterator it = m_coms.begin();
		for (; it != m_coms.end(); ++it) {
			ComObjs::iterator it_obj = it->second.objs.begin();
			for (; it_obj != it->second.objs.end(); ++it_obj) {
				IUnknown* fac = static_cast<IUnknown*>(it_obj->second.ppv);
				if (!fac) {
					assert(0);
					continue;
				}
					
				int32_t refCount = fac->Release();
				if (0 == refCount) {
					// 对象已经完全释放
                    it_obj->second.ppv = NULL;
				} else {
					// 说明还在使用暂时还不能释放
                    fac->AddRef();
                }
			}
		}

		it = m_coms.begin();
        for (; it != m_coms.end(); ++it) {
			ComObjs::iterator it_obj = it->second.objs.begin();
            for (; it_obj != it->second.objs.end(); ++it_obj) {
                IUnknown* fac = static_cast<IUnknown*>(it_obj->second.ppv);
                if (!fac) {
                    continue;
                }

				while(fac->Release() > 0);
            }

            it->second.com->unload();
        }

		m_coms.clear();

		return 0;
	}

	int32_t GnModules::queryInterface(REFCLSID rclsid,REFIID riid,void** ppv)
	{
		Coms::iterator it_clsid = m_coms.find(rclsid);
		if (it_clsid == m_coms.end()) {
			for (int32_t i = 0; i < kMaxNumberCom; ++i) {
				if (rclsid != m_comDatabase[i].clsid) {
					continue;
				}

				GnCom* com = new GnCom();
				int32_t ret = com->load(m_comDatabase[i].name.c_str());
				if (0 == ret) {
					ComItem item = m_comDatabase[i];
					item.com = com;
					m_coms[m_comDatabase[i].clsid] = item;
					it_clsid = m_coms.find(rclsid);
					break;
				} else {
					delete com;
					assert(0);
					return -1;
				}
			}
		}

		if (it_clsid != m_coms.end()) {
			ComObjs::iterator it_riid = it_clsid->second.objs.find(riid);
			if (it_riid == it_clsid->second.objs.end()) {
				if (it_clsid->second.com->createInstance(rclsid,riid,ppv) == 0) {
					ComObjItem item;
					item.riid = riid;
					item.ppv = *ppv;
					((IUnknown*)item.ppv)->AddRef();
					it_clsid->second.objs[riid] = item;
					return 0;
				} else {
					assert(0);
				}
			} else if (SUCCEEDED(((IUnknown*)it_riid->second.ppv)->QueryInterface(riid,(void **)&*ppv))) {
				return 0;
			} else if (!it_riid->second.ppv) {
				assert(0);
			}
		}

		assert(0);
		return -1;		
	}

}  // namespace gn

