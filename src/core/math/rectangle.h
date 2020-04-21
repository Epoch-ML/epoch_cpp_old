#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "core/ehm/result.h"

// epoch math rectangle
// epoch/src/core/math/rectangle.h

template <typename T>
class rectangle {
public:
	rectangle() :
		width(0),
		height(0)
	{}

	rectangle(const T& width, const T& height) :
		w(width),
		h(height)
	{}

	rectangle(const rectangle& rhs)  {
		memcpy(data, rhs.data, sizeof(T) * 2);
	}

	rectangle& operator=(const rectangle& rhs) {
		memcpy(data, rhs.data, sizeof(T) * 2);
		return *this;
	}

	rectangle(rectangle&& rhs) {
		memcpy(data, rhs.data, sizeof(T) * 2);
		rhs.zero();
	}

	rectangle& operator=(rectangle&& rhs) {
		memcpy(data, rhs.data, sizeof(T) * 2);
		
		rhs.zero();
		
		return *this;
	}

	inline T& width() { return w; }
	inline T& height() { return h; }

	inline T& width(const T& val) { return (w = val); }
	inline T& height(const T& val) { return (h = val); }

public:
	RESULT zero() {
		memset(data, 0, sizeof(T) * 2);
		return R::OK;
	}

public:
	union {
		T data[2];
		struct {
			T w;
			T h;
		};
	};
};

#endif // ! RECTANGLE_H_