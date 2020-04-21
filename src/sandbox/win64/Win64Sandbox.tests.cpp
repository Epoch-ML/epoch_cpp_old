#include "sandbox/SandboxTestSuite.h"

#include "test/EPTest.h"
#include "test/EPTestCase.h"

#include "sandbox/sandbox.h"

RESULT SandboxTestSuite::TestWin64Sandbox(EPTestBase* pEPTestBase) {
	RESULT r = R::OK;

	const char* kWin64Window = "win64window";
	const char* kWin64Console = "win64console";
	EPRef<SandboxProcess> pSBProcess = nullptr;

	EPRef<sandbox> pSandbox = sandbox::make();
	CNM(pSandbox, "Failed to make sandbox");

	// Create a win64 sandbox window process
	CRM(pSandbox->CreateSandboxProcess(kWin64Window, SandboxProcess::type::window), "Failed to create win64 sandbox window");
	CNM(pSBProcess = pSandbox->GetSandboxProcess(kWin64Window), "Failed to create win64 sandbox window");
	CRM(pSBProcess->Run(), "Failed to run Win64 Window Sandbox Process");

	// Create a win64 sandbox console process
	//CRM(pSandbox->CreateSandboxProcess(kWin64Console, SandboxProcess::type::console), "Failed to create win64 sandbox console");
	//CNM((pSBProcess = pSandbox->GetSandboxProcess(kWin64Console)), "Failed to create win64 sandbox console");
	//CRM(pSBProcess->Run(), "Failed to run Win64 Window Console Process");


	// Don't quit while thread is running
	while (pSBProcess->IsRunning()) {
		// 
	}

Error:
	return r;
}