#ifndef PROJECTION_H_
#define PROJECTION_H_

#include "core/ehm/result.h"

// epoch math matrix projection
// epoch/src/core/math/matrix/projection.h

// Projection matrix

#include "core/math/matrix/transform.h"

template <typename TValue = float>
class projection :
	public transform<TValue>
{
public:
	enum class type : uint8_t {
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

public:
	projection() {
		clear(1.0f);
	}

	projection(projection::type projectionType, 
		TValue width, TValue height,
		TValue nearPlane, TValue farPlane,
		TValue angleFOV) 
	{
		clear(1.0f);

		switch (projectionType) {
			case type::PERSPECTIVE:  SetPerspective(width, height, nearPlane, farPlane, angleFOV); break;
			case type::ORTHOGRAPHIC: SetOrthographic(width, height, nearPlane, farPlane); break;
		}
	}

	~projection() = default;

	virtual GetType() { return transform::type::PROJECTION; }

	// https://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/
	// https://unspecified.wordpress.com/2012/06/21/calculating-the-gluperspective-matrix-and-other-opengl-matrix-maths/
	RESULT SetPerspective(
		TValue width, TValue height, 
		TValue nearPlane, TValue farPlane, 
		TValue angleFOV) 
	{

		TValue ratio = width / height;
		TValue top = nearPlane * static_cast<TValue>(((M_PI / 180.0f) * (angleFOV / 2.0f)));
		TValue bottom = -top;
		TValue right = top * ratio;
		TValue left = -right;

		TValue f = 1.0f / static_cast<TValue>((tan((M_PI / 180.0f) * (angleFOV / 2.0f))));

		this->element(0, 0) = f / ratio;

		this->element(1, 1) = f;

		this->element(2, 2) = -((farPlane + nearPlane) / (farPlane - nearPlane));
		this->element(2, 3) = -((2.0f * farPlane * nearPlane) / (farPlane - nearPlane));

		this->element(3, 2) = (-1.0f);

		return R::OK;
	}

	// http://www.songho.ca/opengl/gl_projectionmatrix.html
	RESULT SetPerspective(
		TValue left, TValue right,
		TValue top, TValue bottom,
		TValue nearPlane, TValue farPlane)
	{
		this->element(0, 0) = (2.0f * nearPlane) / (right - left);
		this->element(2, 0) = (right + left) / (right - left);

		this->element(1, 1) = (2.0f * nearPlane) / (top - bottom);
		this->element(1, 2) = (top + bottom) / (top - bottom);

		this->element(2, 2) = -((farPlane + nearPlane) / (farPlane - nearPlane));
		this->element(2, 3) = -((2.0f * farPlane * nearPlane) / (farPlane - nearPlane));

		this->element(3, 2) = (-1.0f);

		return R::OK;
	}

	// TODO: add axis (ortho axis X, Y, Z, vector etc)
	// http://www.songho.ca/opengl/gl_projectionmatrix.html
	RESULT SetOrthographic(
		TValue left, TValue right,
		TValue top, TValue bottom,
		TValue nearPlane, TValue farPlane)
	{
		// Scaling / Clipping
		this->element(0, 0) = 2.0f / (right - left);
		this->element(1, 1) = 2.0f / (top - bottom);
		this->element(2, 2) = -2.0f / (farPlane - nearPlane);

		// Translation
		this->element(0, 3) = (-1.0f * (right + left)) / (right - left);
		this->element(1, 3) = (-1.0f * (top + bottom)) / (top - bottom);
		this->element(2, 3) = (-1.0f * (farPlane + nearPlane)) / (farPlane - nearPlane);

		this->element(3, 3) = 1.0f;

		return R::OK;
	}

	RESULT SetOrthographic(
		TValue width, TValue height,
		TValue nearPlane, TValue farPlane)
	{
		// Scaling / Clipping
		this->element(0, 0) = 2.0f / (width);
		this->element(1, 1) = 2.0f / (height);
		this->element(2, 2) = -2.0f / (farPlane - nearPlane);

		// Translation
		this->element(2, 3) = (-1.0f * (farPlane + nearPlane)) / (farPlane - nearPlane);

		this->element(3, 3) = 1.0f;

		return R::OK;
	}
};


#endif // ! PROJECTION_H_