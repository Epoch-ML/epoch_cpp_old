#ifndef EP_TEST_H_
#define EP_TEST_H_

#include "core/ehm/ehm.h"

// epoch test
// epoch/src/test/test.h

#include "core/types/EPFunction.h"


template <typename TReturn, typename ... TArgs>
class EPTest : public EPFunction<TReturn(TArgs...)>
{
public:
	EPTest(TReturn(*pfnTestFunction)(TArgs...)) :
		EPFunction(pfnTestFunction)
	{
		// 
	}

private:
	// 
};

#endif // ! EP_TEST_H_