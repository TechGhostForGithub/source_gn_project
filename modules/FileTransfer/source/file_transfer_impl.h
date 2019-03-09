#ifndef _FILE_TRANSFER_IMPL_H_
#define _FILE_TRANSFER_IMPL_H_
#include "modules/FileTransfer/interface/file_transfer.h"
#include "modules/LogModule/log_module.h"
#pragma comment(lib,"LogModule.dll.lib")

namespace gn
{
	class CFileTransferImpl : public IFileTransfer
	{
	public:
		CFileTransferImpl();
		virtual ~CFileTransferImpl();

	public:
		STDMETHOD_(int32_t,init) (THIS_ GnBase* base);
		STDMETHOD_(int32_t,terminate) (THIS_);
		STDMETHOD_(int32_t,test) (THIS_);

	private:
		GnBase* m_gnBase;
	};
}


#endif // _FILE_TRANSFER_IMPL_H_
