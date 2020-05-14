#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "core/ehm/result.h"

// epoch math triangle
// epoch/src/core/math/triangle.h

template <typename T>
class triangle {
public:
	triangle() :
		a(0), b(0), c(0)
	{}

	triangle(const T& a, const T& b, const T& c) :
		a(a), b(b), c(c)
	{}

	triangle(const rectangle& rhs) {
		memcpy(data, rhs.data, sizeof(data));
	}

	triangle& operator=(const rectangle& rhs) {
		memcpy(data, rhs.data, sizeof(data));
		return *this;
	}

	triangle(rectangle&& rhs) {
		memcpy(data, rhs.data, sizeof(data));
		rhs.zero();
	}

	triangle& operator=(rectangle&& rhs) {
		memcpy(data, rhs.data, sizeof(data));
		rhs.zero();
		return *this;
	}

	inline const T& a() const { return a; }
	inline const T& b() const { return b; }
	inline const T& c() const { return c; }

	inline T& a() { return a; }
	inline T& b() { return b; }
	inline T& c() { return c; }

	inline T& a(const T& val) { return (a = val); }
	inline T& b(const T& val) { return (b = val); }
	inline T& c(const T& val) { return (c = val); }

public:
	RESULT zero() {
		memset(data, 0, sizeof(data));
		return R::OK;
	}

public:
	union {
		T data[3];
		struct {
			T a;
			T b;
			T c;
		};
	};
};

#endif // ! TRIANGLE_H_