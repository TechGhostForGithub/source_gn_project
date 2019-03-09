#ifndef _GN_MODULES_H_
#define _GN_MODULES_H_

#include <map>
#include <vector>
#include <string>
#include <assert.h>
using namespace std;
#include "common_types.h"
#include "modules/ClientKernel/interface/gn_base.h"
#include "modules/ClientKernel/source/gn_modules_win_com.h"
#pragma comment(lib,"LogModule.dll.lib")

//#define LOAD_DLL_FROM_XML

#ifdef _UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif // _UNICODE

namespace gn
{
	// ------------------------------------------
	// ComObjItem
	// ------------------------------------------
	struct ComObjItem
	{
		IID riid;
		void* ppv;
	};

	// ------------------------------------------
	// IIDPred
	// ------------------------------------------
	struct IIDPred
	{
		bool operator()(const IID &x,const IID &y) const
		{
			return memcmp(&x,&y,sizeof(IID)) < 0;
		}
	};
	typedef map<IID,ComObjItem,IIDPred> ComObjs;

	// ------------------------------------------
	// ComItem
	// ------------------------------------------
	struct ComItem 
	{
#ifdef LOAD_DLL_FROM_XML
		ComItem()
		{
			com = NULL;
		}
#endif
		tstring name;
		CLSID clsid;
		GnCom* com;
		ComObjs objs;
	};

	// ------------------------------------------
	// CLSIDPred
	// ------------------------------------------
	struct CLSIDPred
	{
		bool operator()(const CLSID &x,const CLSID &y) const
		{
			return memcmp(&x,&y,sizeof(CLSID)) < 0;
		}
	};
	typedef map<CLSID,ComItem,CLSIDPred> Coms;

	// ------------------------------------------
	// GnModules
	// ------------------------------------------
	class GnModules
	{
		enum { kMaxNumberCom = 15 };
	public:
		GnModules();

		virtual ~GnModules();

		virtual int32_t loadResource();

		virtual int32_t unloadResource();

		virtual int32_t queryInterface(REFCLSID rclsid,REFIID riid,void** ppv);

	private:
		Coms					m_coms;
		static ComItem			m_comDatabase[kMaxNumberCom];
	};
}  // namespace gn

#endif // _HJ_MODULES_H_
