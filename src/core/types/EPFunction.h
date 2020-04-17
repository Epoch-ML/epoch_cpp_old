#ifndef EP_FUNCTION_H_
#define EP_FUNCTION_H_

#include "core/ehm/result.h"

// epoch function
// epoch/src/core/types/EPFunction.h

// Simple functor

#include "core/types/EPObj.h"

#include <cstddef>

template <typename>
class EPFunction;

template <typename TReturn, typename... TArgs>
struct functor_base {
	virtual ~functor_base() = default;
	virtual TReturn call(TArgs...) const = 0;
	
	virtual functor_base* clone() const = 0;
};

template <typename TObject, typename TReturn, typename... TArgs> 
struct functor_impl : public functor_base<TReturn, TArgs...>
{
	using Self = functor_impl<TObject, TReturn, TArgs...>;

	explicit functor_impl(TObject object) :
		m_object(std::move(object))
	{
		//
	}

	TReturn call(TArgs... args) const override {
		return const_cast<TObject&>(m_object)(static_cast<TArgs&&>(args)...);
	}

	functor_base<TReturn, TArgs...>* clone() const override {
		return new Self(*this);
	}

	TObject m_object;
};

template <typename TReturn, typename... TArgs>
class EPFunction<TReturn(TArgs...)> : 
	public EPObj
{
public:
	EPFunction() = default;

	EPFunction(const EPFunction&& rhs) : 
		m_pfnFunction(rhs.m_pfnFunction)
	{
		rhs.m_pfnFunction = nullptr;
	}

	EPFunction(EPFunction& rhs) :
		m_pfnFunction(rhs.m_pfnFunction ? rhs.m_pfnFunction->clone() : nullptr)
	{}

	EPFunction(const EPFunction& rhs) :
		m_pfnFunction(rhs.m_pfnFunction ? rhs.m_pfnFunction->clone() : nullptr)
	{}

	EPFunction(EPFunction&& rhs) noexcept :
		m_pfnFunction(rhs.m_pfnFunction)
	{
		rhs.m_pfnFunction = nullptr;
	}

	EPFunction(std::nullptr_t) noexcept :
		m_pfnFunction(nullptr)
	{
		//
	}

	EPFunction(functor_base<TReturn, TArgs...>* pfnFunction) noexcept :
		m_pfnFunction(pfnFunction ? pfnFunction->clone() : nullptr)
	{
		//
	}

	~EPFunction() {
		if (m_pfnFunction != nullptr) {
			delete m_pfnFunction;
			m_pfnFunction = nullptr;
		}
	}

	template <class Callable, class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	EPFunction(Callable&& object) :
		m_pfnFunction(new functor_impl<typename std::decay<Callable>::type, TReturn, TArgs...>(static_cast<Callable&&>(object)))
	{
		//
	}

	TReturn operator()(TArgs&& ... args) {
		if (m_pfnFunction != nullptr) {
			return m_pfnFunction->call(static_cast<TArgs&&>(args)...);
		}
		else {
			return static_cast<TReturn>(0);
		}
	}

	template <class Callable, class = decltype(TReturn(std::declval<typename std::decay<Callable>::type>()(std::declval<TArgs>()...)))>
	EPFunction& operator=(Callable&& object) {
		m_pfnFunction = new functor_impl<typename std::decay<Callable>::type, TReturn, TArgs...>(static_cast<Callable&&>(object));
		return *this;
	}

	EPFunction& operator=(std::nullptr_t) noexcept {
		m_pfnFunction = nullptr;
		return *this;
	}

	EPFunction& operator=(EPFunction&& rhs) {
		this->m_strName = rhs.m_strName;
		this->m_pfnFunction = rhs.m_pfnFunction;

		rhs.m_pfnFunction = nullptr;

		return *this;
	}
	
	EPFunction& operator=(EPFunction& rhs) noexcept {
		m_pfnFunction = (rhs.m_pfnFunction ? rhs.m_pfnFunction->clone() : nullptr);
		return *this;
	}

	EPFunction& operator=(const EPFunction& rhs) noexcept {
		m_pfnFunction = (rhs.m_pfnFunction ? rhs.m_pfnFunction->clone() : nullptr);
		return *this;
	}

	explicit operator bool() const noexcept {
		return m_pfnFunction != nullptr;
	}

	bool operator==(std::nullptr_t) const noexcept {
		return (m_pfnFunction == nullptr);
	}

protected:
	functor_base<TReturn, TArgs...>* m_pfnFunction = nullptr;
};

#endif // ! EP_FUNCTION_H_