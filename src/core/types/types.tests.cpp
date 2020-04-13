#ifndef TYPES_TEST_CPP_
#define TYPES_TEST_CPP_

#include "core/ehm/ehm.h"

// epoch types unit tests
// epoch/src/core/types/types.tests.cpp

#include "TypesTestSuite.h"

int main(int argc, char* argv[]) {
	RESULT r = R::SUCCESS;

	TypesTestSuite *pTypesTestSuite = TypesTestSuite::make();
	CRM(pTypesTestSuite->RunAllTests(), "Failed to run all tests");

Success:
	return 0;

Error:
	return -1;
}


#endif // ! TYPES_TEST_CPP_