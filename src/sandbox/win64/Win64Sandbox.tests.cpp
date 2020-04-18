#include "sandbox/SandboxTestSuite.h"

#include "test/EPTest.h"
#include "test/EPTestCase.h"

#include "sandbox/sandbox.h"

RESULT SandboxTestSuite::TestWin64Sandbox(EPTestBase* pEPTestBase) {
	RESULT r = R::OK;

	EPRef<sandbox> pSandbox = sandbox::make();
	CNM(pSandbox, "Failed to make sandbox");



Error:
	return r;
}