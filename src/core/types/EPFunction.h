#ifndef EP_FUNCTION_H_
#define EP_FUNCTION_H_

#include "core/ehm/result.h"

// epoch function
// epoch/src/core/types/EPFunction.h

// Simple functor

#include "core/types/EPObj.h"


template <typename TReturn, typename ... TArgs>
class EPFunction<TReturn(TArgs...) : public EPObj {
public:
	~EPFunction() = default;


};