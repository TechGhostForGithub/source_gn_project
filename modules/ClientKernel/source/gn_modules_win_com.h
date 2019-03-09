#ifndef _GN_COM_H_
#define _GN_COM_H_

#include "common_types.h"
#include "comdefs.h"
#include <atlcomcli.h>

namespace gn
{
	typedef HRESULT ( __stdcall * DLLGETCLASSOBJECT)(const CLSID& clsid,const IID& iid,void** ppv);
	class SharedData;
	class GnCom {
	public:
		GnCom();

		virtual ~GnCom();

		virtual int32_t load(LPCTSTR lpDLL);

		virtual int32_t unload();

		virtual int32_t createInstance(const CLSID& clsid,const IID& iid,void** ppv);

		static void getPath(TCHAR szPath[MAX_PATH]);
		static void getPath(TCHAR szPath[MAX_PATH],LPCTSTR lpFilename);
	private:
		HMODULE					_hDLL;
		DLLGETCLASSOBJECT		_lpDllGetClassObject;
	};
}  // namespace gn

#endif // _GN_COM_H_
