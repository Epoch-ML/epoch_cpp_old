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

	#ifdef EPOCH_HAL_VULKAN
		RESULT TestVulkanHAL(EPTestBase* pEPTestBase);
	#endif

	EP_TESTS_START{
		
		#ifdef EPOCH_HAL_VULKAN
			EP_ADD_TEST(TestVulkanHAL);
		#endif

	} EP_TESTS_END
};


#endif // HAL_TEST_SUITE_H_