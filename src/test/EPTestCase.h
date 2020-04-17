#ifndef EP_TEST_CASE_H_
#define EP_TEST_CASE_H_

#include "core/ehm/ehm.h"

// epoch test case
// epoch/src/test/EpTestCase.h

#include "core/types/EPString.h"
#include "core/types/EPTimedFunction.h"
#include "core/types/EPTuple.h"

#include <chrono>

class EPTestCase {
public:
	enum class expected {
		FASTEST,
		SLOWEST,
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

	static EPTestCase Make(const EPString<char>& strTestCaseName, const EPString<char>& strTestCaseFlavor, EPTimedFunction<RESULT(void)> pfnFunction) {
		EPTestCase retTestCase;

		retTestCase.m_strTestCaseName = strTestCaseName;
		retTestCase.m_strTestCaseFlavor = strTestCaseFlavor;
		retTestCase.m_epTimedFunction = EPTimedFunction<RESULT(void)>(pfnFunction);

		return retTestCase;
	}

	static EPTestCase MakeAndRun(const EPString<char> &strTestCaseName, const EPString<char> &strTestCaseFlavor, EPTimedFunction<RESULT(void)> pfnFunction) {
		EPTestCase retTestCase = EPTestCase::Make(strTestCaseName, strTestCaseFlavor, pfnFunction);
		
		retTestCase.Run();

		return retTestCase;
	}

	RESULT GetResult() { return m_epTimedFunction.GetResult(); }
	bool Failed() { return m_epTimedFunction.Failed(); }
	bool Succeeded() { return (m_epTimedFunction.Succeeded()); }

	RESULT PrintTestCase() {
		RESULT r = R::OK;

		CRM(GetResult(), "TestCase failed");

		DEBUG_LINEOUT("%s: %s: %zu", 
			m_strTestCaseName.c_str(), m_strTestCaseFlavor.c_str(), GetTestCaseDuration());

	Error:
		return r;
	}

	// Compare test cases
	static RESULT CompareTestCases(const EPVector<EPTestCase> &cases) {
		RESULT r = R::OK;

		EPTestCase* pLHS = nullptr;
		EPTestCase* pRHS = nullptr;

		CBM(cases.size() != 0, "No test cases registered");
		CBM(cases.size() <= 2, "CompareTestCases doesn't yet support more han two comparable test cases");

		pLHS = const_cast<EPTestCase*>(&(cases[0]));
		if (cases.size() == 1) {
			return pLHS->PrintTestCase();
		}
		pRHS = const_cast<EPTestCase*>(&(cases[1]));

		CRM(pLHS->GetResult(), "lhs failed");
		CRM(pRHS->GetResult(), "rhs failed");

		// Print the comparison
		DEBUG_CMP(pLHS->m_strTestCaseName.c_str(),
			pLHS->m_strTestCaseFlavor.c_str(), pLHS->GetTestCaseDuration(),
			pRHS->m_strTestCaseFlavor.c_str(), pRHS->GetTestCaseDuration());

		expected lhsExpected;

		// Defer to LHS to be the fastest, if both are trying to be faster 
		// then go with the one that actually is trying to be faster
		if (pLHS->m_expectedBehavior == expected::FASTEST) {
			lhsExpected = expected::FASTEST;
		}
		else if (pRHS->m_expectedBehavior == expected::FASTEST) {
			lhsExpected = expected::SLOWEST;
		}
		else {
			lhsExpected = expected::COMPARE;
		}

		switch (lhsExpected) {
			case expected::COMPARE: {
				//// Print the comparison
				//DEBUG_CMP(lhs.m_strTestCaseName.c_str(),
				//	lhs.m_strTestCaseFlavor.c_str(), lhs.GetTestCaseDuration(),
				//	rhs.m_strTestCaseFlavor.c_str(), rhs.GetTestCaseDuration());
			} break;

			case expected::FASTEST: {
				CLTDM(pLHS->GetTestCaseDuration(), pRHS->GetTestCaseDuration(),
					pLHS->m_strTestCaseName.c_str(),
					"%s: %s: %zu ns %s: %zu ns",
						pLHS->m_strTestCaseName.c_str(), 
						pLHS->m_strTestCaseFlavor.c_str(), pLHS->GetTestCaseDuration(),
					pRHS->m_strTestCaseFlavor.c_str(), pRHS->GetTestCaseDuration()
				);
			} break;

			case expected::SLOWEST: {
				CGTDM(pLHS->GetTestCaseDuration(), pRHS->GetTestCaseDuration(),
					pLHS->m_strTestCaseName.c_str(),
					"%s: %s: %zu ns %s: %zu ns",
					pLHS->m_strTestCaseName.c_str(),
					pLHS->m_strTestCaseFlavor.c_str(), pLHS->GetTestCaseDuration(),
					pRHS->m_strTestCaseFlavor.c_str(), pRHS->GetTestCaseDuration()
				);
			} break;	
		}

	Error:
		return r;
	}

	RESULT SetExpectedBehavior(EPTestCase::expected expectedBehavior) {
		m_expectedBehavior = expectedBehavior;
		return R::OK;
	}

private:
	EPTestCase::expected m_expectedBehavior = EPTestCase::expected::COMPARE;
	EPString<char> m_strTestCaseName;
	EPString<char> m_strTestCaseFlavor;
	EPTimedFunction<RESULT(void)> m_epTimedFunction = nullptr;
};

#endif // ! EP_TEST_CASE_H_