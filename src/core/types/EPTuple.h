#ifndef EP_TUPLE_H_
#define EP_TUPLE_H_

#include "core/ehm/result.h"

// epoch function
// epoch/src/core/types/EPTuple.h

#include <utility>
#include <type_traits>

#include "core/types/EPObj.h"

template <size_t index, typename T>
class tuple_implementation {
public:
	tuple_implementation() :
		m_value()
	{}

	tuple_implementation(T const& lval) :
		m_value(lval)
	{}

	tuple_implementation(T&& lval) {
		m_value = std::move(lval);
	}

	// Defaults
	~tuple_implementation() = default;
	tuple_implementation& operator= (const tuple_implementation&) = default;
	tuple_implementation& operator= (tuple_implementation&&) = default;

	T& get() {
		return m_value;
	}

private:
	T m_value;
};

// Base
template <size_t index, typename... TArgs>
class tuple_recurring_base
{
	// 
};

// Specialization
template <size_t index, typename T, typename... TArgs>
class tuple_recurring_base<index, T, TArgs...> : 
	public tuple_implementation<index, T>,
	public tuple_recurring_base<index + 1, TArgs...>
{
public:
	tuple_recurring_base() 
	{
		//
	}

	template<typename CType, typename... CArgs>
	tuple_recurring_base(CType &&arg, CArgs && ... args) :
		tuple_implementation<index, CType>(std::forward<CType>(arg)),
		tuple_recurring_base<index + 1, CArgs...>(std::forward<CArgs>(args)...)
	{
		// 
	}

};


template <typename... TArgs>
class tuple : public tuple_recurring_base<0, TArgs...>
{
public:
	tuple() 
	{
		// 
	}

	template <typename... CArgs>
	tuple(CArgs && ... args) :
		tuple_recurring_base<0, TArgs...>(std::forward<CArgs>(args)...)
	{
		// 
	}

	template <size_t index, typename GType>
	GType& get() {
		return static_cast<tuple_implementation<index, GType>*>(this)->get();
	}

private:
	
};

#endif // ! EP_TUPLE_H_