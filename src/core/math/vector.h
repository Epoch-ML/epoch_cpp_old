#ifndef VECTOR_H_
#define VECTOR_H_

#include "core/ehm/result.h"

// epoch vector
// epoch/src/core/math/vector.h

#include "core/math/matrix/matrix.h"

template <typename TValue = float, int N = 4>
class vector :
	public matrix<TValue, N, 1>
{
public:
	vector() = default;
	~vector() = default;

	vector(std::initializer_list<TValue> values) {
		//memcpy(data, &values, sizeof(data));

		int index = 0;
		for (auto val : values) {
			data[index++] = val;
		}
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

#endif // ! POINT_H_