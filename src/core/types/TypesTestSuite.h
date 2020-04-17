#ifndef TYPES_TEST_SUITE_H_
#define TYPES_TEST_SUITE_H_

#include "core/ehm/ehm.h"

// epoch types unit tests
// epoch/src/core/types/types.tests.cpp

#include "test/EPTestSuite.h"

#include <functional>

class TypesTestSuite :
	public EPTestSuite<TypesTestSuite>
{	

public:
	// EPDynamicStorage
	RESULT TestEPDynamicStorage(EPTestBase* pEPTestBase);
	RESULT TestEPString(EPTestBase* pEPTestBase);

	EP_TESTS_START {

		EP_ADD_TEST(TestEPDynamicStorage);
		EP_ADD_TEST(TestEPString);

	} EP_TESTS_END
};


#endif // TYPES_TEST_SUITE_H_