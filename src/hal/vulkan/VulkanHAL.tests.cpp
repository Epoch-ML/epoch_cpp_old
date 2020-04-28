#include "hal/HalTestSuite.h"

#include "test/EPTest.h"
#include "test/EPTestCase.h"

#include "sandbox/sandbox.h"
#include "hal/hal.h"

#include "hal/HALFactory.h"

RESULT HALTestSuite::TestVulkanHAL(EPTestBase* pEPTestBase) {
	RESULT r = R::OK;
	
	// TODO: Need to make this cross-platform since the VulkanHAL test
	// should work on a variety of platforms
	const char* kWin64Window = "win64window";
	const char* kWin64Console = "win64console";
	SandboxProcess::type sandboxType = SandboxProcess::type::window;
	sandbox::platform sandboxPlatform = sandbox::platform::win64;
	HAL::type halType = HAL::type::vulkan;

	EPRef<SandboxProcess> pSBProcess = nullptr;
	EPRef<HAL> pVulkanHAL = nullptr;

	EPRef<sandbox> pSandbox = sandbox::make();
	CNM(pSandbox, "Failed to make sandbox");

	// Create a win64 sandbox window process
	CRM(pSandbox->CreateSandboxProcess(kWin64Window, sandboxType), "Failed to create win64 sandbox window");
	CNM(pSBProcess = pSandbox->GetSandboxProcess(kWin64Window), "Failed to create win64 sandbox window");
	CRM(pSBProcess->Run(), "Failed to run Win64 Window Sandbox Process");

	//// Create a win64 sandbox console process
	//CRM(pSandbox->CreateSandboxProcess(kWin64Console, SandboxProcess::type::console), "Failed to create win64 sandbox console");
	//CNM((pSBProcess = pSandbox->GetSandboxProcess(kWin64Console)), "Failed to create win64 sandbox console");
	//CRM(pSBProcess->Run(), "Failed to run Win64 Window Console Process");

	pVulkanHAL = HALFactory::make(halType, pSBProcess);
	CNM(pVulkanHAL, "Failed to create VulkanHAL");


	// Don't quit while thread is running
	while (pSandbox->GetRunningProcesses().size() != 0) {
		// 
	}


Error:
	return r;
}