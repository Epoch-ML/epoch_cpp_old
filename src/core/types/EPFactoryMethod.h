#ifndef EP_FACTORY_METHOD_H_
#define EP_FACTORY_METHOD_H_

#include "core/ehm/ehm.h"

// epoch factory
// epoch/src/core/types/EPProcess.h

// A factory pattern

#include <type_traits>

#include "core/types/EPTuple.h"

#include "core/types/EPObj.h"
#include "core/types/epref.h"

template <typename TObj, typename ... MArgs>
class EPFactoryMethod {
protected:
	EPFactory() = delete;
	~EPFactory() = delete;

public:
	static EPRef<TObj> make(MArgs... args) {
		RESULT r = R::OK;

		EPRef<TObj> pEPObj = TObj::InternalMake(args...);
		CNM(pEPObj, "Factory method failed to create " CSTR(TObj));

	Success:
		return pEPObj;

	Error:
		pEPObj = nullptr;
		return nullptr;
	}

protected:
	static EPRef<TObj> InternalMake(MArgs...) {

		// This needs to be overridden, but this is a 
		// syntax hack since it's not possible to virtualize static functions

		return nullptr;
	}
};

#endif // EP_FACTORY_METHOD_H_