#include "common_types.h"
#include "comdefs.h"
#include "modules/LogModule/log_module.h"
#include "modules/ClientKernel/source/gn_modules_posix.h"
#include "modules/ClientKernel/interface/gn_base.h"
#include "modules/MessageKernel/interface/message_kernel.h"
#include "modules/MessageSync/interface/message_sync.h"
#include "modules/NetworkService/interface/network_service.h"
#include "modules/HttpKernel/interface/http_kernel.h"
#include "modules/UserFace/interface/user_face.h"
#include "modules/DataCenter/interface/data_center.h"
#include "modules/DataReport/interface/data_report.h"
#include "modules/ImageKernel/interface/image_kernel.h"
#include "modules/FileTransfer/interface/file_transfer.h"
#include "modules/FileModule/interface/file_module.h"
#include "modules/EngineModule/interface/engine_module.h"
#include "modules/NetworkRouting/interface/network_routing.h"
#include "modules/H5OfflinePkg/interface/h5_offline_pkg.h"
#include "modules/CommonHead/interface/common_head.h"
#include <assert.h>
namespace gn
{
    GnModules::GnModules()
    {
        
    }
    
    GnModules::~GnModules()
    {
        
    }
    
    int32_t GnModules::loadResource()
    {
        return 0;
    }
    
    int32_t GnModules::unloadResource()
    {
        if (m_modules.size() == 0)
        {
            GN_TRACE(kTraceWarning, kTraceClientKernel, 0, "%s resource size is 0",__FUNCTION__);
            return -1;
        }
        
        // 第一次遍历：释放那些未被其他对象包含的对象
        // 第二次遍历：释放那些未释放的对象, 并且卸载相应的模块
        // 之所以两次循环，是为了处理相互包含的情况
		Coms::iterator it = m_modules.begin();
        for (; it != m_modules.end(); ++it) {
			ComObjs::iterator it_obj = it->second.objs.begin();
            for (; it_obj != it->second.objs.end(); ++it_obj) {
                IUnknown* fac = static_cast<IUnknown*>(it_obj->second.ppv);
				if (!fac) {
					assert(0);
				}
                
				int32_t uRef = fac->Release();
                if (0 == uRef == 0) {
                    // 对象已经完全释放
                    it_obj->second.ppv = NULL;
                } else {
                    // 说明还在使用暂时还不能释放
                    fac->AddRef();
                }
            }
        }
        
		it = m_modules.begin();
        for (; it != m_modules.end(); ++it) {
			ComObjs::iterator it_obj = it->second.objs.begin();
            for (; it_obj != it->second.objs.end(); ++it_obj) {
                IUnknown* fac = static_cast<IUnknown*>(it_obj->second.ppv);
                if (!fac) {
                    continue;
                }

				while(fac->Release() > 0);
            }
        }
        
        m_modules.clear();
        
        return 0;
    }
    
    int32_t GnModules::queryInterface(REFCLSID rclsid,REFIID riid,void** ppv)
    {
		Coms::iterator it_clsid = m_modules.find(rclsid);
		if (it_clsid != m_modules.end()) {
			ComObjs::iterator it_riid = it_clsid->second.objs.find(riid);
			if (it_riid != it_clsid->second.objs.end()) {
				if (((IUnknown*)it_riid->second.ppv)->QueryInterface(riid,(void **)&*ppv) >= 0) {
					return 0;
				}
				assert(0);
				return -1;
			} else {
				if (createInstance(rclsid,riid,ppv) == 0) {
					ComObjItem item;
					item.riid = riid;
					item.ppv = *ppv;
                    ((IUnknown*)item.ppv)->AddRef();

					it_clsid->second.objs[riid] = item;
					return 0;
				}
				assert(0);
				return -1;
			}
		} else {
			if (createInstance(rclsid,riid,ppv) == 0) {
				ComObjItem item;
				item.riid = riid;
				item.ppv = *ppv;
				((IUnknown*)item.ppv)->AddRef();

				ComItem cls_item;
				cls_item.clsid = rclsid;
				cls_item.objs[riid] = item;
				m_modules[rclsid] = cls_item;
				return 0;
			}
		}

		assert(0);
		return -1;
	}

	int32_t GnModules::createInstance(REFCLSID rclsid,REFIID riid,void** ppv)
	{
		if (rclsid == CLSID_NetworkService) {
			return NetworkService_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_MessageSync) {
			return MessageSync_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_HttpKernel) {
            return HttpKernel_CreateObject(rclsid, riid, ppv);

        } else if (rclsid == CLSID_MessageKernel) {
			return MessageKernel_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_DataCenter) {
			return DataCenter_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_FileModule) {
			return FileModule_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_FileTransfer) {
			return FileTransfer_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_UserFace) {
			return UserFace_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_NetworkRouting) {
			return NetworkRouting_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_ImageKernel) {
			return ImageKernel_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_EngineModule) {
			return EngineModule_CreateObject(rclsid, riid, ppv);

		} else if (rclsid == CLSID_DataReport) {
            return DataReport_CreateObject(rclsid, riid, ppv);

        } else if (rclsid == CLSID_H5OfflinePkg) {
            return H5OfflinePkg_CreateObject(rclsid, riid, ppv);

        } else if (rclsid == CLSID_CommonHead) {
            return CommonHead_CreateObject(rclsid, riid, ppv);

        } else {
			assert(0);
			return -1;
		}
	}
}

