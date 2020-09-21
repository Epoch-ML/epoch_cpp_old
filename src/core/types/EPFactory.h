#ifndef EP_FACTORY_H_
#define EP_FACTORY_H_

#include "core/ehm/ehm.h"

// epoch factory
// epoch/src/core/types/EPProcess.h

// A factory pattern

#include <type_traits>

#include "core/types/EPTuple.h"

#include "core/types/EPObj.h"
#include "core/types/EPRef.h"

template <typename TEPFactory, typename TEPObj, typename ... MArgs>
class EPFactory : 
	public EPObj
	// TODO: singleton?
	
{
protected:
	EPFactory() {
		// 
	}

	virtual ~EPFactory() {
		// 
	}

public:
	static EPRef<TEPObj> make(MArgs... args) {
		RESULT r = R::OK;

		EPRef<TEPObj> pEPObj = TEPFactory::InternalMake(args...);
		CNM(pEPObj, "Failed to create factory object");

	Success:
		return pEPObj;

	Error:
		//pEPObj = nullptr;
		return nullptr;
	}

protected:
	static EPRef<TEPObj> InternalMake(MArgs...) {

		// This needs to be overridden, but this is a 
		// syntax hack since it's not possible to virtualize static functions

		return nullptr;
	}
};

#endif // EP_FACTORY_H_