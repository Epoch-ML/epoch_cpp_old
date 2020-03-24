#ifndef EP_TEST_SUITE_H_
#define EP_TEST_SUITE_H_

#include "core/ehm/ehm.h"

// epoch test
// epoch/src/test/test.h

#include "core/types/EPObj.h"
#include "test/EPTest.h"

#include <vector>

class EPTestSuite :
	public EPObj
{
public:
	

private:
	std::vector<EPTest<RESULT()>> m_tests;
};

#endif // !EP_TEST_SUITE_H_