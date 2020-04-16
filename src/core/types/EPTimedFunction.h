#ifndef EP_TIMED_TEST_H_
#define EP_TIMED_TEST_H_

#include "core/ehm/ehm.h"

// epoch test
// epoch/src/test/test.h

#include "core/types/EPFunction.h"
#include "core/types/EPTuple.h"

#include <chrono>

template <typename T>
class EPTimedFunction;

template <typename TReturn, typename... TArgs>
class EPTimedFunction<TReturn(TArgs...)> : public EPObj
{
public:
	// TODO: This should go into a more general utility area
	enum class duration_type : uint8_t {
		//years
		//months,
		//days,
		hours,
		minutes,
		seconds,
		milliseconds,
		microseconds,
		nanoseconds
	};

public:
	EPTimedFunction() :
		EPFunction()
	{
		//
	}

public:
	EPTuple<RESULT, std::duration> operator()(TArgs&& ... args) {
		RESULT r = R::OK;
		m_fnResult = R::FAIL;

		CN(m_pfnFunction);
		CR(Run(static_cast<TArgs&&>(args)...));

	Error:
		return EPTuple<RESULT, size_t>(m_fnResult, m_duration);
	}

	template <class Callable, class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	EPTimedFunction& operator=(Callable&& object) {
		m_pfnFunction = new functor_impl<typename std::decay<Callable>::type, TReturn, TArgs...>(static_cast<Callable&&>(object));
		return *this;
	}

	template <class Callable, class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	EPTimedFunction(Callable&& object) :
		m_pfnFunction(new functor_impl<typename std::decay<Callable>::type, TReturn, TArgs...>(static_cast<Callable&&>(object)))
	{
		//
	}

	explicit operator bool() const noexcept {
		return m_pfnFunction != nullptr;
	}

	bool operator==(std::nullptr_t) const noexcept {
		return (m_pfnFunction == nullptr);
	}

	RESULT GetResult() { return m_fnResult; }
	bool Failed() { return RSUCCESS(m_fnResult); }
	bool Succeeded() { return (m_fnResult); }

	template <class Callable, 
		class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	static EPTuple<RESULT, std::duration> MakeAndRun(Callable&& object, TArgs... args) {
		EPTimedFunction<TReturn(TArgs...)> epTimedFunction(object);
		return epTimedFunction(static_cast<TArgs&&>(args)...);
	}

private:
	template <typename... CArgs>
	RESULT Run(CArgs... args) {
		RESULT r = R::OK;

		auto timeStart = std::chrono::high_resolution_clock::now();
			m_pfnFunction->call(static_cast<CArgs&&>(args)...);
		auto timeEnd = std::chrono::high_resolution_clock::now();

		m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart);

	Error:
		return r;
	}

	functor_base<TReturn, TArgs...>* m_pfnFunction = nullptr;
	RESULT m_fnResult = R::NOT_RESULT;
	std::duration m_duration = 0;
};

template <typename ... TArgs, typename... CArgs>
EPTimedFunction<RESULT(TArgs...)>::Run(CArgs... args) {
	RESULT r = R::OK;

	auto timeStart = std::chrono::high_resolution_clock::now();
	m_fnResult = m_pfnFunction->call(static_cast<CArgs&&>(args)...);
	auto timeEnd = std::chrono::high_resolution_clock::now();

	m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart);

Error:
	return r;
}

#endif // ! EP_TIMED_FUNCTION_H_