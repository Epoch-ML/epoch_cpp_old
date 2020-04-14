#ifndef EP_FUNCTION_PACK_H_
#define EP_FUNCTION_PACK_H_

#include "core/ehm/result.h"

// epoch function
// epoch/src/core/types/EPFunction.h

// Simple functor

#include "core/types/EPObj.h"

#include <core/types/EPTuple.h>

template <typename>
EPFunctionPack;

template <typename TReturn, typename... TArgs>
class EPFunctionPack<TReturn(TArgs...)>  {


private:
	EPTuple<TArgs...> m_functionData;
};

#endif // ! EP_FUNCTION_PACK_H_