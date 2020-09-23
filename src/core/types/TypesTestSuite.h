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
	RESULT TestEPVector(EPTestBase* pEPTestBase);
	RESULT TestEPString(EPTestBase* pEPTestBase);
	RESULT TestEPList(EPTestBase* pEPTestBase);
	RESULT TestEPRef(EPTestBase* pEPTestBase);
	RESULT TestEPTuple(EPTestBase* pEPTestBase);

	EP_TESTS_START {

		EP_ADD_TEST(TestEPVector);
		EP_ADD_TEST(TestEPList);

		EP_ADD_TEST(TestEPRef);

		// TODO: this is incomplete
		EP_ADD_TEST(TestEPString);

		EP_ADD_TEST(TestEPTuple);

	} EP_TESTS_END
};


#endif // TYPES_TEST_SUITE_H_