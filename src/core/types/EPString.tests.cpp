#include "TypesTestSuite.h"


RESULT TypesTestSuite::TestEPString(EPTestBase* pEPTestBase) {
	RESULT r = R::OK;

	const char* kTestString = "Test string";
	const wchar_t* kTestWideString = L"Test wide string";

	EPString<char> strTestString = kTestString;

	// TODO: wide string
	//EPString<wchar_t> wstrTestString = kTestWideString;

	EPTest<RESULT(EPTestBase*)>* pEPTest = dynamic_cast<EPTest<RESULT(EPTestBase*)>*>(pEPTestBase);
	CNM(pEPTest, "EPTest is nullptr");

	// Case 1 - 
	CBM(strTestString == kTestString, "String mismatch");
	//CBM(wstrTestString == kTestWideString, "Wide string mismatch");

Error:
	return r;
}