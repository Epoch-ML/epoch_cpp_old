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

	vector(TValue x, TValue y, TValue z) { x(x); y(y); z(z); w(0.0f); }

	vector(std::initializer_list<TValue> values) {
		//memcpy(data, &values, sizeof(data));

		int index = 0;
		for (auto val : values) {
			data[index++] = val;
		}
	}

	vector(const matrix<TValue, N, 1>& rhs) {
		memcpy(this->data, rhs.data, sizeof(data));
	}
	
	vector& operator=(const matrix<TValue, N, 1>& rhs) {
		memcpy(this->data, rhs.data, sizeof(data));
		return *this;
	}

	vector(matrix<TValue, N, 1>&& rhs) {
		memcpy(this->data, rhs.data, sizeof(data));
		rhs.clear();
	}

	vector& operator=(matrix<TValue, N, 1>&& rhs) {
		memcpy(this->data, rhs.data, sizeof(data));
		rhs.clear();
		return *this;
	}

	// Cross product constructor
	vector(const vector& lhs, const vector& rhs) {
		this->clear();

		x((rhs(1) * lhs(2)) - (rhs(2) * lhs(1)));
		y((rhs(2) * lhs(0)) - (rhs(0) * lhs(2)));
		z((rhs(0) * lhs(1)) - (rhs(1) * lhs(0)));
	}

	RESULT set(std::initializer_list<TValue> values) {
		//memcpy(data, &values, sizeof(data));

		int index = 0;
		for (auto val : values) {
			data[index++] = val;
		}

		return R::OK;
	}

	RESULT invert() {
		this->data[0] *= -1.0f;
		this->data[1] *= -1.0f;
		this->data[2] *= -1.0f;

		return R::OK;
	}

	static vector cross(const vector &lhs, const vector &rhs) {
		return vector(lhs, rhs);
	}

	static TValue dot(const vector& lhs, const vector& rhs) {
		TValue dotProd = 0.0f;

		for (uint8_t i = 0; i < 4; i++) {
			dotProd += lhs.data[i] * rhs.data[i];
		}

		return dotProd;
	}
	
	TValue dot(const vector& rhs) {
		TValue dotProd = 0.0f;

		for (uint8_t i = 0; i < 4; i++) {
			dotProd += this->data[i] * rhs.data[i];
		}

		return dotProd;
	}

	vector cross(const vector& rhs) {
		return vector(*this, rhs);
	}

	inline TValue& x() { return this->data[0]; }
	inline TValue& y() { return this->data[1]; }
	inline TValue& z() { return this->data[2]; }
	inline TValue& w() { return this->data[3]; }

	inline TValue& x(const TValue &val) { return this->data[0] = val; }
	inline TValue& y(const TValue &val) { return this->data[1] = val; }
	inline TValue& z(const TValue &val) { return this->data[2] = val; }
	inline TValue& w(const TValue &val) { return this->data[3] = val; }

	static vector i(TValue value = 1.0f) { return vector(value, 0.0f, 0.0f); }
	static vector j(TValue value = 1.0f) { return vector(0.0f, value, 0.0f); }
	static vector k(TValue value = 1.0f) { return vector(0.0f, 0.0f, value); }

};

#endif // ! POINT_H_