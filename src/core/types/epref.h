#ifndef EP_REF_H_
#define EP_REF_H_

#include "core/ehm/result.h"

// epoch reference
// epoch/src/core/types/epref.h

// A simple referenced count object

#include "core/types/EPObject.h"

#include <type_traits>

template <typename TEPObj> requires EPObjectDerived<TEPObj>
class epref {
public:
	EPProcess() {
		// 
	}

	~EPProcess() {
		// 
	}
};

#endif // EP_REF_H_