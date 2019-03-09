#ifndef _GN_MODULES_NORMAL_H_
#define _GN_MODULES_NORMAL_H_
#include "common_types.h"
#include "comdefs.h"
#include <map>

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
typedef std::map<IID,ComObjItem,IIDPred> ComObjs;

// ------------------------------------------
// ComItem
// ------------------------------------------
struct ComItem 
{
	CLSID clsid;
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
typedef std::map<CLSID,ComItem,CLSIDPred> Coms;


// ------------------------------------------
// GnModules
// ------------------------------------------
namespace gn
{
    class GnModules
	{
	public:
		GnModules();
        
		virtual ~GnModules();
        
		virtual int32_t loadResource();
        
		virtual int32_t unloadResource();

		virtual int32_t queryInterface(REFCLSID rclsid,REFIID riid,void** ppv);

	private:
		virtual int32_t createInstance(REFCLSID rclsid,REFIID riid,void** ppv);

	private:
		Coms	m_modules;
	};
}

#endif // _GN_MODULES_NORMAL_H_
