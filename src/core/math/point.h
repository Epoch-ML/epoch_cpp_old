#ifndef POINT_H_
#define POINT_H_

#include "core/ehm/result.h"

// epoch point
// epoch/src/core/math/point.h

#include "core/math/matrix/matrix.h"

template <typename TValue = float, int N = 4>
class point :
	public matrix<TValue, N, 1>
{
public:
	point() = default;
	~point() = default;

	point(TValue x, TValue y, TValue z) { x(x); y(y); z(z); w(1.0f); }

	point(std::initializer_list<TValue> values) {
		// TODO: this should work / be faster
		//memcpy(data, &values, sizeof(data));

		int index = 0;			
		for (auto val : values) {
			data[index++] = val;
		}
	}

	point& operator=(std::initializer_list<TValue> values) {
		//memcpy(data, &values, sizeof(data));

		int index = 0;
		for (auto val : values) {
			data[index++] = val;
		}

		return *this;
	}

	RESULT set(std::initializer_list<TValue> values) {
		//memcpy(data, &values, sizeof(data));

		int index = 0;
		for (auto val : values) {
			data[index++] = val;
		}

		return R::OK;
	}

	inline TValue& x() { return this->data[0]; }
	inline TValue& y() { return this->data[1]; }
	inline TValue& z() { return this->data[2]; }
	inline TValue& w() { return this->data[3]; }

	inline TValue& x(const TValue &val) { return this->data[0] = val; }
	inline TValue& y(const TValue &val) { return this->data[1] = val; }
	inline TValue& z(const TValue &val) { return this->data[2] = val; }
	inline TValue& w(const TValue &val) { return this->data[3] = val; }

};

//// Not sure if this will work
//template <typename TValue>
//point<TValue, 4>::point(TValue x, TValue y, TValue z, TValue w) {
//	data[0] = x;
//	data[1] = y;
//	data[2] = z;
//	data[3] = w;
//}



#endif // ! POINT_H_