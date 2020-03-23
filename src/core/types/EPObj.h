#ifndef EP_OBJECT_H_
#define EP_OBJECT_H_

#include "core/ehm/result.h"

// epoch types: EPObject
// epoch/src/core/types/epobject.h

// This is the base object for epoch

#include "core/types/valid.h"

#include "core/types/epuid.h"

#ifndef NOCONCEPTS
	template <class T>
	concept EPObjectDerived = std::is_base_of<typename EPObject, T>::value;
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