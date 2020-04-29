#include "TypesTestSuite.h"


RESULT TypesTestSuite::TestEPRef(EPTestBase* pEPTestBase) {
	RESULT r = R::OK;

	EPTest<RESULT(EPTestBase*)>* pEPTest = dynamic_cast<EPTest<RESULT(EPTestBase*)>*>(pEPTestBase);
	CNM(pEPTest, "EPTest is nullptr");

	// Case 3 - Test performance against std::string

Error:
	return r;
}