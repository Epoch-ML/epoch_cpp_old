#ifndef EP_TEST_H_
#define EP_TEST_H_

#include "core/ehm/ehm.h"

// epoch test
// epoch/src/test/test.h

#include "core/types/EPFunction.h"
#include "core/types/EPString.h"

template <typename T>
class EPTest;

template <typename TReturn, typename... TArgs>
class EPTest<TReturn(TArgs...)> : 
	public EPFunction<TReturn(TArgs...)>
{
public:
	EPTest() :
		EPFunction()
		//m_strName(),
		//m_testResult(R::FAIL),
		//m_fTestRun(false)
	{
		//
	}

	EPTest(const EPTest&& rhs) :
		EPFunction(rhs)
	{}

	EPTest(const EPTest& rhs) :
		m_strName(rhs.m_strName),
		m_testResult(rhs.m_testResult),
		m_fTestRun(rhs.m_fTestRun),
		EPFunction(rhs.m_pfnFunction)
	{}
	
	EPTest(EPFunction& rhs) :
		m_strName(rhs.m_strName),
		m_testResult(rhs.m_testResult),
		m_fTestRun(rhs.m_fTestRun),
		EPFunction(rhs.m_pfnFunction)
	{}

	EPTest(const EPFunction& rhs) :
		EPTest(),
		EPFunction(rhs.m_pfnFunction)
	{}


	EPTest(EPFunction&& rhs) noexcept :
		EPFunction(rhs.m_pfnFunction),
		m_strName(),
		m_testResult(R::FAIL),
		m_fTestRun(false),
	{}

	EPTest(std::nullptr_t) noexcept :
		EPFunction(nullptr),
		m_strName(),
		m_testResult(R::FAIL),
		m_fTestRun(false)
	{}

	template <class Callable, class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	EPTest(const char szName[], Callable&& object) :
		m_strName(szName),
		EPFunction(object)
	{}
	
	EPTest& operator=(std::nullptr_t) noexcept {
		m_pfnFunction = nullptr;
		return *this;
	}

public:
	RESULT Run() {
		RESULT r = R::OK;

		m_fTestRun = true;
		m_testResult = this->operator()();

	Error:
		return r;
	}

	const EPString<char>& GetName() {
		return m_strName;
	}

	template <class Callable, class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
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
	EPString<char> m_strName;
	RESULT m_testResult = R::OK;
	bool m_fTestRun = false;
};

#endif // ! EP_TEST_H_