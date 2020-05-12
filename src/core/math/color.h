#ifndef COLOR_H_
#define COLOR_H_

#include "core/ehm/result.h"

// epoch color
// epoch/src/core/math/color.h

#include "core/math/matrix/matrix.h"

class color :
	public matrix<float, 4, 1>
{
public:
	color() = default;
	~color() = default;

	color(float r, float g, float b, float a) :
		matrix<float, 4, 1>({ r, g, b, a })
	{
		//
	}

	color(std::initializer_list<float> values) :
		matrix<float, 4, 1>(values)
	{
		//
	}

	color& operator=(std::initializer_list<float> values) {
		//memcpy(data, &values, sizeof(data));

		int index = 0;
		for (auto val : values) {
			data[index++] = val;
		}

		return *this;
	}

	RESULT set(std::initializer_list<float> values) {
		memcpy(data, &values, sizeof(data));
		return R::OK;
	}

	inline float& r() { return this->data[0]; }
	inline float& g() { return this->data[1]; }
	inline float& b() { return this->data[2]; }
	inline float& a() { return this->data[3]; }

	inline float& r(const float &val) { return this->data[0] = val; }
	inline float& g(const float &val) { return this->data[1] = val; }
	inline float& b(const float &val) { return this->data[2] = val; }
	inline float& a(const float &val) { return this->data[3] = val; }

};

#endif // ! COLOR_H_