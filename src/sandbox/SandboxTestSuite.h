#ifndef SANDBOX_TEST_SUITE_H_
#define SANDBOX_TEST_SUITE_H_

#include "core/ehm/ehm.h"

// epoch Sandbox Test suite
// epoch/src/sandbox/SandboxTestSuite.h

#include "test/EPTestSuite.h"

class SandboxTestSuite :
	public EPTestSuite<SandboxTestSuite>
{

public:
	RESULT TestWin64Sandbox(EPTestBase* pEPTestBase);

	EP_TESTS_START{

		EP_ADD_TEST(TestWin64Sandbox);

	} EP_TESTS_END
};


#endif // TYPES_TEST_SUITE_H_