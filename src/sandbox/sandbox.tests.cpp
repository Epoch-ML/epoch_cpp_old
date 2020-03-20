#ifndef SANDBOX_TEST_CPP_
#define SANDBOX_TEST_CPP_

#include "core/ehm/ehm.h"

// epoch sandbox unit tests
// epoch/src/sandbox/sandbox.tests.cpp

#include <functional>

#include <sandbox/sandbox.h>

std::function<RESULT()> SandboxImpTest = []() -> RESULT {
	RESULT r = R::SUCCEED;

	sandbox* pSandbox = nullptr;

Error:
	return r;
};

int main(int argc, char* argv[]) {
	RESULT r = R::SUCCESS;

	CRM(SandboxImpTest(), "SandboxImpTest failed");

Success:
	return 0;

Error:
	return -1;
}


#endif // ! SANDBOX_TEST_CPP_