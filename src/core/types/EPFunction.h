#ifndef EP_FUNCTION_H_
#define EP_FUNCTION_H_

#include "core/ehm/result.h"

// epoch function
// epoch/src/core/types/EPFunction.h

// Simple functor

#include "core/types/EPObj.h"

template <typename>
class EPFunction;

template <typename TReturn, typename... TArgs>
class EPFunction<TReturn(TArgs...)> : 
	public EPObj
{
public:
	EPFunction() :
		m_pfnFunction(nullptr)
	{
		//
	}

	EPFunction(TReturn(*pfnFunction)(TArgs...)) :
		m_pfnFunction(pfnFunction)
	{
		// 
	}

	~EPFunction() = default;

	TReturn operator()(TArgs&& ... args) {
		if (m_pfnFunction != nullptr) {
			return m_pfnFunction(args...);
		}
		else {
			return static_cast<TReturn>(0);
		}
	}

	template <typename Fx>
	EPFunction<TReturn(TArgs...)>& operator=(const Fx& fn) {
		m_pfnFunction = fn;
		return *this;
	}
	

private:
	TReturn(*m_pfnFunction)(TArgs...) = nullptr;
};

#endif // ! EP_FUNCTION_H_