#include "modules/ClientKernel/source/gn_modules_win_com.h"
#include "modules/ClientKernel/source/gn_modules_win_com_def.h"
#include <Shlwapi.h>
#include <shlobj.h>
//#pragma comment( lib, "shell32.lib")
//#pragma comment( lib, "shlwapi.lib")
#pragma warning(disable:4996 4995)

namespace gn
{
	GnCom::GnCom(void)
	{
		_hDLL					= NULL;
		_lpDllGetClassObject	= NULL;
	}

	GnCom::~GnCom(void)
	{
		unload();
	}

	int32_t GnCom::load(LPCTSTR lpDLL)
	{
		if(!_hDLL) {
			TCHAR szPath[MAX_PATH] = {0};
			getPath(szPath,lpDLL);

			// 加载DLL并获取对象
			_hDLL = LoadLibrary(szPath);
			if(_hDLL != NULL) {
				// 定义类型并且获得导出函数首地址
				_lpDllGetClassObject = (DLLGETCLASSOBJECT)GetProcAddress(_hDLL, "DllGetClassObject");
			}

			return _lpDllGetClassObject != NULL ? 0 : -1;
		}
		return 0;
	}

	int32_t GnCom::createInstance(const CLSID& clsid,const IID& iid,void** ppv)
	{
		if (_lpDllGetClassObject) {
			CComPtr<IClassFactory> lpFactory;
			if (SUCCEEDED(_lpDllGetClassObject(clsid,IID_IClassFactory,(void**)&lpFactory))) {
				return SUCCEEDED(lpFactory->CreateInstance(NULL, iid, ppv)) ? 0 : -1;
			}
		}
		return -1;
	}

	int32_t GnCom::unload()
	{
		if (_hDLL) {
			_lpDllGetClassObject = NULL;
			if(!FreeLibrary(_hDLL)) {
				return -1;
			}
			_hDLL = NULL;
		}
		return 0;
	}

	void GnCom::getPath(TCHAR szPath[MAX_PATH])
	{
		::GetModuleFileName(g_hModule, szPath, MAX_PATH);
		::PathRemoveFileSpec(szPath);
		::PathAddBackslash(szPath);
	}

	void GnCom::getPath(TCHAR szPath[MAX_PATH],LPCTSTR lpFilename)
	{
		getPath(szPath);
		::PathAppend(szPath, lpFilename);
	}
}
#pragma warning(default:4996 4995)
