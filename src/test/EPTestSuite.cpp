#include "EPTestSuite.h"

#include <string>

RESULT EPTestSuiteBase::Add(std::string strTestName, EPTest<RESULT()> eptest) {
	RESULT r = R::OK;
	
	// TODO: build own printf fn support c strings or use different string class 
	CBM((m_tests.find(strTestName) == m_tests.end()), "Test %s already exists, test names in suites need to be unique", strTestName.c_str());

	m_tests[strTestName] = eptest;

Error:
	return r;
}

RESULT EPTestSuiteBase::Run(std::string strTestName) {
	RESULT r = R::OK;

	CBM((m_tests.find(strTestName) != m_tests.end()), "Test %s not found", strTestName.c_str());

	CRM(m_tests[strTestName].Run(), "Test %s failed", strTestName.c_str());

Error:
	return r;
}

EPTest<RESULT()> EPTestSuiteBase::FindTest(std::string strTestName) {
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
		r = epTest.second.Run();

		EPTest<RESULT()> test = epTest.second;
		EPString<char> strName = test.GetName();
		const char* pszName = strName.c_str();

		CRM(r, "Failed to run test %s", pszName);

		DEBUG_LINEOUT("Test: %s : %s with code 0x%x", 
			pszName, 
			RSUCCESS(test.GetResult()) ? "\x1B[32mSUCCEEDED\033[0m" : "\x1B[31mFAILED\033[0m", 
			test.GetResult()
		);

		// If one of the tests failed flag it as a failure
		if (test.Failed()) 
			fAnyFailedTests = true;
	}

	CBM(fAnyFailedTests, "Not all tests passed");

Error:
	return r;
}