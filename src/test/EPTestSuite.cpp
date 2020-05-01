#include "EPTestSuite.h"

#include <string>

RESULT EPTestSuiteBase::Add(std::string strTestName, EPTest<RESULT(EPTestBase*)> eptest) {
	RESULT r = R::OK;
	
	// TODO: build own printf fn support c strings or use different string class 
	CBM((m_tests.find(strTestName) == m_tests.end()), "Test %s already exists, test names in suites need to be unique", strTestName.c_str());

	m_tests[strTestName] = eptest;

Error:
	return r;
}

RESULT EPTestSuiteBase::Run(std::string strTestName) {
	RESULT r = R::OK;
	EPTestBase* pEpTestBase = nullptr;

	CBM((m_tests.find(strTestName) != m_tests.end()), "Test %s not found", strTestName.c_str());

	pEpTestBase = dynamic_cast<EPTestBase*>(&(m_tests[strTestName]));
	CNM(pEpTestBase, "Failed to retrieve EPTestBase handle");

	CRM(m_tests[strTestName].Run(pEpTestBase), "Test %s failed", strTestName.c_str());

Error:
	return r;
}

EPTest<RESULT(EPTestBase *)> EPTestSuiteBase::FindTest(std::string strTestName) {
	auto epTest_it = m_tests.find(strTestName);

	if (epTest_it != m_tests.end()) {
		return (*epTest_it).second;
	}
	else {
		return nullptr;
	}
}

RESULT EPTestSuiteBase::RunAllTests() {
	RESULT r = R::OK;

	bool fAnyFailedTests = false;

	// TODO: The below is super verbose unclear what's busted that makes 
	// this required for the compiler

	for (auto& epTest : m_tests) {

		EPTestBase* pEpTestBase = dynamic_cast<EPTestBase*>(&(epTest.second));
		CNM(pEpTestBase, "Failed to retrieve EPTestBase handle");

		r = epTest.second.Run(pEpTestBase);

		EPTest<RESULT(EPTestBase *)> test = epTest.second;
		EPString<char> strName = test.GetName();
		const char* pszName = strName.c_str();

		CRM(r, "Failed to run test %s", pszName);

		RELEASE_LINEOUT("Test: %s : %s with code 0x%x", 
			pszName, 
			RSUCCESS(test.GetResult()) ? C_GREEN("SUCCEEDED") : C_RED("FAILED"), 
			test.GetResult()
		);

		// If one of the tests failed flag it as a failure
		if (test.Failed()) 
			fAnyFailedTests = true;
	}

	CBM(fAnyFailedTests == false, "Not all tests passed");

Error:
	return r;
}