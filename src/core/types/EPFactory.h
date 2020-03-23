#ifndef EP_FACTORY_H_
#define EP_FACTORY_H_

#include "core/ehm/ehm.h"

// epoch factory
// epoch/src/core/types/EPProcess.h

// A factory pattern

#include <type_traits>

#include "core/types/EPObject.h"
#include "core/types/epref.h"

// TODO: Fix this, C++20!!
#ifndef NOCONCEPTS
	template <typename TEPObj> requires EPObjectDerived<TEPObj>
#else
	template <typename TEPObj>
#endif
class EPFactory : 
	public EPObject
	// TODO: singleton?
	
{
private:
	EPFactory() {
		// 
	}

	~EPFactory() {
		// 
	}

public:
	virtual static epref<TEPObj> make() = 0;
};

#endif // EP_FACTORY_H_