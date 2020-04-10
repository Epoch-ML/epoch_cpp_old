#ifndef EP_FUNCTION_H_
#define EP_FUNCTION_H_

#include "core/ehm/result.h"

// epoch function
// epoch/src/core/types/EPFunction.h

// Simple functor

#include "core/types/EPObj.h"

//template <typename>
//class EPFunction;

template <typename TCapture, typename... TArgs>
class EPFunction :
	public EPFunction<decltype(&TCapture::operator())(TArgs...)>
{
	//
};


template <typename CObject>
class EPFunction<CObject> :
	public EPObj
{
private:
	//TReturn(*m_pfnFunction)(TArgs...) = nullptr;
	CObject m_object;

public:
	//EPFunction() :
	//	m_pfnFunction(nullptr)
	//{
	//	//
	//}

	EPFunction(const CObject& cObject) :
		m_object(cObject)
	{
		//
	}

	~EPFunction() = default;

	template<typename... Args> typename 
	std::result_of<CObject(Args...)>::type operator()(Args... args) {
		return this->m_object.operator()(Args...);
	}

	template<typename... Args> typename 
	std::result_of<CObject(Args...)>::type operator()(Args... args) const {
		return this->m_object.operator()(Args...);
	}

	//template <typename Fx>
	//EPFunction<TReturn(TArgs...)>& operator=(const Fx& fn) {
	//	m_pfnFunction = fn;
	//	return *this;
	//}

};

#endif // ! EP_FUNCTION_H_