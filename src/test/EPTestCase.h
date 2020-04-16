#ifndef EP_TEST_CASE_H_
#define EP_TEST_CASE_H_

#include "core/ehm/ehm.h"

// epoch test case
// epoch/src/test/EpTestCase.h

#include "core/types/EPFunction.h"
#include "core/types/EPString.h"
#include "core/types/EPTimedFunction.h"
#include "core/types/EPTuple.h"

#include <chrono>

class EPTestCase {
public:
	enum class expected {
		LT,
		LTE,
		GT,
		GTE,
		EQ,
		FASTER,
		SLOWER,
		COMPARE		// This will not error out
	};

public:
	EPTestCase() = default;
	~EPTestCase() = default;

public:
	RESULT Run() {
		RESULT r = R::OK;

		// TODO: 
		{
			CNM(m_epTimedFunction, "Timed function cannot be nullptr");

			EPTuple<RESULT, size_t> resultTuple = m_epTimedFunction();

			r = resultTuple.get<0, RESULT>();
			CRM(r, "Timed function call failed");
		}

	Error:
		return r;
	}

	const EPString<char>& GetName() {
		return m_strTestCaseName;
	}

	size_t GetTestCaseDuration() {
		return m_epTimedFunction.GetDuration();
	}

	static EPTestCase Make(const char szTestCaseName[], const char szTestCaseFlavor[], EPTimedFunction<RESULT(void)> pfnFunction) {
		EPTestCase retTestCase;

		retTestCase.m_strTestCaseName = szTestCaseName;
		retTestCase.m_strTestCaseFlavor = szTestCaseFlavor;
		retTestCase.m_epTimedFunction = EPTimedFunction<RESULT(void)>(pfnFunction);

		return retTestCase;
	}

	static EPTestCase MakeAndRun(const char szTestCaseName[], const char szTestCaseFlavor[], EPTimedFunction<RESULT(void)> pfnFunction) {
		EPTestCase retTestCase = EPTestCase::Make(szTestCaseName, szTestCaseFlavor, pfnFunction);
		
		retTestCase.Run();

		return retTestCase;
	}

	RESULT GetResult() { return m_epTimedFunction.GetResult(); }
	bool Failed() { return m_epTimedFunction.Failed(); }
	bool Succeeded() { return (m_epTimedFunction.Succeeded()); }

	// Compare test cases
	static RESULT CompareTestCases(EPTestCase lhs, EPTestCase rhs, EPTestCase::expected lhsExpected) {
		RESULT r = R::OK;

		CRM(lhs.GetResult(), "lhs failed");
		CRM(rhs.GetResult(), "rhs failed");

		// Print the comparison
		DEBUG_CMP(lhs.m_strTestCaseName.c_str(),
			lhs.m_strTestCaseFlavor.c_str(), lhs.GetTestCaseDuration(),
			rhs.m_strTestCaseFlavor.c_str(), rhs.GetTestCaseDuration());

		switch (lhsExpected) {
			case EPTestCase::expected::COMPARE: {
				//// Print the comparison
				//DEBUG_CMP(lhs.m_strTestCaseName.c_str(),
				//	lhs.m_strTestCaseFlavor.c_str(), lhs.GetTestCaseDuration(),
				//	rhs.m_strTestCaseFlavor.c_str(), rhs.GetTestCaseDuration());
			} break;

			//case EPTestCase::expected::LT:
			//case EPTestCase::expected::LTE:
			//case EPTestCase::expected::GT:
			//case EPTestCase::expected::GTE:
			//case EPTestCase::expected::EQ:

			case EPTestCase::expected::FASTER: {
				CLTDM(lhs.GetTestCaseDuration(), rhs.GetTestCaseDuration(),
					lhs.m_strTestCaseName.c_str(),
					"%s: %s: %zu ns %s: %zu ns",
						lhs.m_strTestCaseName.c_str(), 
						lhs.m_strTestCaseFlavor.c_str(), lhs.GetTestCaseDuration(), 
						rhs.m_strTestCaseFlavor.c_str(), rhs.GetTestCaseDuration()
				);
			} break;

			case EPTestCase::expected::SLOWER: {
				CGTDM(lhs.GetTestCaseDuration(), rhs.GetTestCaseDuration(),
					lhs.m_strTestCaseName.c_str(),
					"%s: %s: %zu ns %s: %zu ns",
					lhs.m_strTestCaseName.c_str(),
					lhs.m_strTestCaseFlavor.c_str(), lhs.GetTestCaseDuration(),
					rhs.m_strTestCaseFlavor.c_str(), rhs.GetTestCaseDuration()
				);
			} break;	
		}

	Error:
		return r;
	}

private:
	EPString<char> m_strTestCaseName;
	EPString<char> m_strTestCaseFlavor;
	EPTimedFunction<RESULT(void)> m_epTimedFunction = nullptr;
};

#endif // ! EP_TEST_CASE_H_