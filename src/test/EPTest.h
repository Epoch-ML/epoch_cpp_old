#ifndef EP_TEST_H_
#define EP_TEST_H_

#include "core/ehm/ehm.h"

// epoch test
// epoch/src/test/test.h

#include "core/types/EPFunction.h"
#include "core/types/EPString.h"

template <typename T>
class EPTest;

template <typename TReturn, typename... TArgs>
class EPTest<TReturn(TArgs...)> : public EPFunction<TReturn(TArgs...)>
{
public:
	EPTest() = default;

	


private:
	EPString m_strName;
};

#endif // ! EP_TEST_H_