#include "EPTestSuite.h"

#include <string>

RESULT EPTestSuite::Add(std::string strTestName, EPTest<RESULT()> eptest) {
	RESULT r = R::OK;
	
	// TODO: build own printf fn support c strings or use different string class 
	CBM((m_tests.find(strTestName) == m_tests.end()), "Test %s already exists, test names in suites need to be unique", strTestName.c_str());

	m_tests[strTestName] = eptest;

Error:
	return r;
}

RESULT EPTestSuite::Run(std::string strTestName) {
	RESULT r = R::OK;

	CBM((m_tests.find(strTestName) != m_tests.end()), "Test %s not found", strTestName.c_str());

	// TODO: CRM(m_tests[strTestName](), "Test %s failed", strTestName.c_str());

Error:
	return r;
}