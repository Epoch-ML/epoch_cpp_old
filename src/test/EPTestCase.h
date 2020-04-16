#ifndef EP_TEST_CASE_H_
#define EP_TEST_CASE_H_

#include "core/ehm/ehm.h"

// epoch test case
// epoch/src/test/EpTestCase.h

#include "core/types/EPFunction.h"
#include "core/types/EPString.h"
#include "core/types/EPTimedFunction.h"

#include <chrono>

class EPTestCase {
public:
	EPTestCase() = default;
	~EPTestCase() = default;

public:
	RESULT Run() {
		RESULT r = R::OK;

		CNM(m_pfnFunction, "Timed function cannot be nullptr");
		
		auto resultTuple = m_epTimedFunction();

		CRM(resultTuple.get<0, RESULT>(), "Timed function call failed");

	Error:
		return r;
	}

	const EPString<char>& GetName() {
		return m_strName;
	}

	size_t GetTestCaseDuration(EPTimedFunction::duration_type dt) {
		switch (dt) {
			case EPTimedFunction<TReturn(TArgs...)>::duration_type::seconds: {
				return std::chrono::duration_cast<std::chrono::seconds> resultTuple.get<0, RESULT>();
			} break;

			case EPTimedFunction<TReturn(TArgs...)>::duration_type::milliseconds: {
				return std::chrono::duration_cast<std::chrono::milliseconds> resultTuple.get<0, RESULT>();
			} break;

			case EPTimedFunction<TReturn(TArgs...)>::duration_type::microseconds: {
				return std::chrono::duration_cast<std::chrono::microseconds> resultTuple.get<0, RESULT>();
			} break;

			case EPTimedFunction<TReturn(TArgs...)>::duration_type::nanoseconds: {
				return std::chrono::duration_cast<std::chrono::nanoseconds> resultTuple.get<0, RESULT>();
			} break;
		}
		
		return std::chrono::duration_cast<std::chrono::microseconds> resultTuple.get<0, RESULT>();
	}

	template <class Callable, class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	static EPTestCase Make(const char* szName, Callable&& object) {
		EPTestCase retTestCase;
		retTestCase.m_epTimedFunction = EPTimedFunction<Callable>(static_cast<Callable&&>(object));

		return retTestCase;
	}

	template <class Callable, class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	static EPTestCase MakeAndRun(const char* szName, Callable&& object) {
		EPTestCase retTestCase = EPTestCase::Make(static_cast<Callable&&>(object));
		
		retTestCase.Run();

		return retTestCase;
	}

	RESULT GetResult() { return m_testResult; }
	bool Failed() { return RSUCCESS(m_testResult); }
	bool Succeeded() { return (m_testResult); }

private:
	EPString<char> m_strName;
	EPTimedFunction<RESULT(void)> m_epTimedFunction = nullptr;
};

#endif // ! EP_TEST_CASE_H_