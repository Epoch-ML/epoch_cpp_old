#ifndef EP_TEST_H_
#define EP_TEST_H_

#include "core/ehm/ehm.h"

// epoch test
// epoch/src/test/test.h

#include "core/types/EPFunction.h"
#include "core/types/EPString.h"
#include "core/types/EPTimedFunction.h"

#include "test/EPTestCase.h"

#include <chrono>
#include <map>

class EPTestBase {
public:
	EPTestBase() = default;
	virtual ~EPTestBase() = default;

	EPTestBase(const char szName[]) :
		m_strName(szName),
		m_testResult(R::NO_RESULT),
		m_fTestRun(false),
		m_usTimeRun(0)
	{}

	EPTestBase(const char szName[], RESULT r, bool fTestRun) :
		m_strName(szName),
		m_testResult(r),
		m_fTestRun(fTestRun),
		m_usTimeRun(0)
	{}

	EPTestBase(const EPString<char> &strName, RESULT r, bool fTestRun) :
		m_strName(strName),
		m_testResult(r),
		m_fTestRun(fTestRun),
		m_usTimeRun(0)
	{}

	virtual RESULT RegisterAndRunTC(const EPString<char> &strTestCaseName,
		const EPString<char> &strTestCaseFlavor, 
		EPTimedFunction<RESULT(void)> pfnFunction) = 0;

protected:
	void reset() {
		RESULT m_testResult = R::NO_RESULT;
		m_strName = EPString<char>();
		m_fTestRun = false;
		m_usTimeRun = 0;
	}

protected:
	EPString<char> m_strName;
	RESULT m_testResult = R::NO_RESULT;
	bool m_fTestRun = false;
	size_t m_usTimeRun = 0;
};

template <typename T>
class EPTest;

template <typename TReturn, typename... TArgs>
class EPTest<TReturn(TArgs...)> : 
	public EPTestBase,
	public EPFunction<TReturn(TArgs...)>
{
public:
	EPTest() :
		EPTestBase(),
		EPFunction()
	{
		//
	}

	EPTest(const EPTest&& rhs) :
		EPFunction(rhs)
	{}

	EPTest(const EPTest& rhs) :
		EPTestBase(rhs.m_strName, rhs.m_testResult, rhs.m_fTestRun),
		EPFunction(rhs.m_pfnFunction)
	{}
	
	EPTest(EPFunction& rhs) :
		EPTestBase(rhs.m_strName, rhs.m_testResult, rhs.m_fTestRun),
		EPFunction(rhs.m_pfnFunction)
	{}

	EPTest(const EPFunction& rhs) :
		EPTestBase(rhs.m_strName, rhs.m_testResult, rhs.m_fTestRun),
		EPFunction(rhs.m_pfnFunction)
	{}


	EPTest(EPFunction&& rhs) noexcept :
		EPTestBase(rhs.m_strName, rhs.m_testResult, rhs.m_fTestRun),
		EPFunction(rhs.m_pfnFunction),
	{
		rhs.reset();
	}

	EPTest(std::nullptr_t) noexcept :
		EPTestBase(),
		EPFunction(nullptr)
	{}

	template <class Callable, 
		class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	EPTest(const char szName[], Callable&& object) :
		EPTestBase(szName),
		EPFunction(object)
	{}
	
	EPTest& operator=(std::nullptr_t) noexcept {
		m_pfnFunction = nullptr;
		return *this;
	}

public:

	RESULT Run(TArgs... args) {
		RESULT r = R::OK;

		std::chrono::steady_clock::time_point timeStart;
		std::chrono::steady_clock::time_point timeEnd;

		CNM(m_pfnFunction, "Cannot run null EPTest");

		timeStart = std::chrono::high_resolution_clock::now();
			m_testResult = m_pfnFunction->call(static_cast<TArgs&&>(args)...);
		timeEnd = std::chrono::high_resolution_clock::now();

		m_usTimeRun = std::chrono::duration_cast<std::chrono::microseconds>(timeEnd - timeStart).count();
		m_fTestRun = true;

	Error:
		return r;
	}

	const EPString<char>& GetName() {
		return m_strName;
	}

public:
	
	virtual RESULT RegisterAndRunTC(const EPString<char>& strTestCaseName,
		const EPString<char>& strTestCaseFlavor,
		EPTimedFunction<RESULT(void)> pfnFunction) override
	{
		RESULT r = R::OK;

		// Create and run the test case
		auto epTestCase = EPTestCase::MakeAndRun(strTestCaseName, strTestCaseFlavor, pfnFunction);

		// Push into test case vector
		if (m_testCases.find(strTestCaseName) == m_testCases.end())
			m_testCases[strTestCaseName] = EPVector<EPTestCase>();

		m_testCases[strTestCaseName].PushBack(epTestCase);

	Error:
		return r;
	}

public:
	template <class Callable, 
		class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	EPTest& operator=(Callable&& object) {
		m_strName = "anonymous function";
		m_pfnFunction = new functor_impl<typename std::decay<Callable>::type, TReturn, TArgs...>(static_cast<Callable&&>(object));
		return *this;
	}

	EPTest& operator=(EPTest& rhs) {
		m_strName = rhs.m_strName;
		m_pfnFunction = (rhs.m_pfnFunction ? rhs.m_pfnFunction->clone() : nullptr);
		return *this;
	}

	EPTest& operator=(const EPTest& rhs) {
		m_strName = rhs.m_strName;
		m_pfnFunction = (rhs.m_pfnFunction ? rhs.m_pfnFunction->clone() : nullptr);
		return *this;
	}

	EPTest& operator=(EPTest&& rhs) {
		m_strName = rhs.m_strName;
		m_pfnFunction = rhs.m_pfnFunction;
		rhs.m_pfnFunction = nullptr;

		return *this;
	}

	explicit operator bool() const noexcept {
		return m_pfnFunction != nullptr;
	}

	bool operator==(std::nullptr_t) const noexcept {
		return (m_pfnFunction == nullptr);
	}

	RESULT GetResult() { return m_testResult; }
	bool Failed() { return RSUCCESS(m_testResult); }
	bool Succeeded() { return (m_testResult); }

private:
	std::map<EPString<char>, EPVector<EPTestCase>, EPString<char>::compare_LT> m_testCases;
};

#endif // ! EP_TEST_H_