#ifndef HAL_TEST_SUITE_H_
#define HAL_TEST_SUITE_H_

#include "core/ehm/ehm.h"

// epoch HAL Test suite
// epoch/src/hal/HALTestSuite.h

#include "test/EPTestSuite.h"

class HALTestSuite :
	public EPTestSuite<HALTestSuite>
{

public:
	RESULT TestVulkanHAL(EPTestBase* pEPTestBase);

	EP_TESTS_START{

		EP_ADD_TEST(TestVulkanHAL);

	} EP_TESTS_END
};


#endif // HAL_TEST_SUITE_H_