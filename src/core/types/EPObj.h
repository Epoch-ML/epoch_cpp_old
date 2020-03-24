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
	concept EPObjectDerived = std::is_base_of<typename EPObj, T>::value;
#else
	#define EPObjectDerived 
#endif

class EPObj :
	public valid
{
public:
	EPObj() = default;
	virtual ~EPObj() = default;
	EPObj(const EPObj&) = default;
	EPObj(EPObj&&) = default;

	EPObj& operator= (const EPObj&) = default;
	EPObj& operator= (EPObj&&) = default;

protected:
	epuid m_epuid;
};

#endif // ! EP_OBJECT_H_