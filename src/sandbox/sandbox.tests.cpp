#ifndef SANDBOX_TEST_CPP_
#define SANDBOX_TEST_CPP_

#include "core/ehm/ehm.h"

// epoch sandbox unit tests
// epoch/src/sandbox/sandbox.tests.cpp

#include <functional>

std::function<RESULT()> SandboxImpTest = []() -> RESULT {
	return R::SUCCEED;
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