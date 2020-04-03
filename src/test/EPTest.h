#ifndef EP_TEST_H_
#define EP_TEST_H_

#include "core/ehm/ehm.h"

// epoch test
// epoch/src/test/test.h

#include "core/types/EPFunction.h"

template <typename T>
class EPTest;

template <typename TReturn, typename... TArgs>
class EPTest<TReturn(TArgs...)> : public EPFunction<TReturn(TArgs...)>
{
public:
	EPTest() :
		EPFunction(nullptr)
	{
		//
	}


	EPTest(TReturn(*pfnTestFunction)(TArgs...)) :
		EPFunction(pfnTestFunction)
	{
		// 
	}

	template <typename Fx>
	EPTest<TReturn(TArgs...)> operator=(Fx pfn) {
		
		//m_pfnFunction = static_cast<TReturn(TArgs...)>(fn);

		//return *this;

		return EPTest<TReturn(TArgs...)>(pfn);
	}

private:
	// 
};

#endif // ! EP_TEST_H_