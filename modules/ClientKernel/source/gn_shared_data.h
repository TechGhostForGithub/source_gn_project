#ifndef _SHARED_DATA_H_
#define _SHARED_DATA_H_

namespace gn
{
    class GnCore;
	class KernelSharedData
	{
	public:
		KernelSharedData();
		virtual ~KernelSharedData();
		
		GnCore* Core();

	protected:
		GnCore* m_gnCore;
	};

}  // namespace webrtc

#endif // _SHARED_DATA_H_
