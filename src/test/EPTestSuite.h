#ifndef EP_TEST_SUITE_H_
#define EP_TEST_SUITE_H_

#include "core/ehm/ehm.h"

// epoch test
// epoch/src/test/test.h

#include "core/types/EPObj.h"

#include "test/EPTest.h"

#include <map>

class EPTestSuite :
	public EPObj
{
public:
	RESULT Add(std::string strTestName, EPTest<RESULT()> eptest);
	RESULT Run(std::string strTestName);

private:
	std::map<std::string, EPTest<RESULT()>> m_tests;
};

#endif // !EP_TEST_SUITE_H_