#ifndef _USER_FACE_IMPL_H_
#define _USER_FACE_IMPL_H_
#include "modules/UserFace/interface/user_face.h"

namespace gn
{
	class CUserFaceImpl : public IUserFace
	{
	public:
		CUserFaceImpl();
		virtual ~CUserFaceImpl();

	public:
		STDMETHOD_(int32_t, init) (THIS_ GnBase* base);
        STDMETHOD_(int32_t, terminate) (THIS_);
		STDMETHOD_(int32_t, test) (THIS_);

	private:
        GnBase*	m_gnBase;
	};
}
#endif // _USER_FACE_IMPL_H_
