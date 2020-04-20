#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "core/ehm/result.h"

// epoch math rectangle
// epoch/src/core/math/rectangle.h

template <typename T>
class rectangle {
public:
	rectangle() :
		m_width(0),
		m_height(0)
	{}

	rectangle(const T& width, const T& height) :
		m_width(width),
		m_height(height)
	{}

	rectangle(const rectangle& rhs) :
		m_width(rhs.m_width),
		m_height(rhs.m_height)
	{}

	rectangle& operator=(const rectangle& rhs) {
		m_width = rhs.m_width;
		m_height = rhs.m_height;
		return *this;
	}

	rectangle(rectangle&& rhs) :
		m_width(rhs.m_width),
		m_height(rhs.m_height)
	{
		rhs.zero();
	}

	rectangle& operator=(rectangle&& rhs) {
		m_width = rhs.m_width;
		m_height = rhs.m_height;
		
		rhs.zero();
		
		return *this;
	}


public:
	RESULT zero() {
		m_width = 0;
		m_height = 0;

		return R::OK;
	}

private:
	T m_width;
	T m_height;
};

#endif // ! RECTANGLE_H_