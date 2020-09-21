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
	#ifdef EPOCH_SANDBOX_WIN32
		RESULT TestWin64Sandbox(EPTestBase* pEPTestBase);
	#endif

	EP_TESTS_START {

		#ifdef EPOCH_SANDBOX_WIN32
			EP_ADD_TEST(TestWin64Sandbox);
		#endif

	} EP_TESTS_END
};


#endif // TYPES_TEST_SUITE_H_