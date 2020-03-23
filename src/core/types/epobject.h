#ifndef EP_OBJECT_H_
#define EP_OBJECT_H_

#include "core/ehm/result.h"

// epoch types: EPObject
// epoch/src/core/types/epobject.h

// This is the base object for epoch

#include "core/types/valid.h"
#include "core/types/epuid.h"

//#if (__cplusplus > 199711L) || (_MSC_VER >= 1300)
#if false
	template <class T>
	concept EPObjectDerived = requires { std::is_base_of<typename EPObject, T>::value};
#else
	#define EPObjectDerived
#endif

class EPObject :
	public valid
{


protected:
	epuid m_epuid;
};

#endif // ! EP_OBJECT_H_