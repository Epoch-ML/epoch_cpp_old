#ifndef ROTATION_H_
#define ROTATION_H_

#include "core/ehm/result.h"

// epoch math matrix rotation
// epoch/src/core/math/matrix/rotation.h

// Rotation matrix

#include "core/math/matrix/rotation.h"

#include "core/math/vector.h"
#include "core/math/math.h"



template <typename TValue = float>
class rotation :
	public transform<TValue>
{
public:
	rotation() {
		this->identity(1.0f);
	}

	scale(math::axis axis, TValue theta) {
		this->clear();

		
	}

	RESULT SetRotationMatrix(math::axis axis, TValue theta) {
		switch (axis) {
			case math::axis::X:    return SetRotationMatrix(theta, 0.0f, 0.0f); break;
			case math::axis::Y:    return SetRotationMatrix(0.0f, theta, 0.0f); break;
			case math::axis::Z:    return SetRotationMatrix(0.0f, 0.0f, theta); break;
		}

		return R::NOT_IMPLEMENTED;
	}

	// https://en.wikipedia.org/wiki/Rotation_formalisms_in_three_dimensions
	RESULT SetRotationMatrix(TValue phi, TValue theta, TValue psi) {
		TValue cosPhi = static_cast<TValue>(cos(phi));
		TValue sinPhi = ROTATION_HAND_SIGN * static_cast<TValue>(sin(phi));

		TValue cosTheta = static_cast<TValue>(cos(theta));
		TValue sinTheta = ROTATION_HAND_SIGN * static_cast<TValue>(sin(theta));

		TValue cosPsi = static_cast<TValue>(cos(psi));
		TValue sinPsi = ROTATION_HAND_SIGN * static_cast<TValue>(sin(psi));

		this->element(0, 0) = cosTheta * cosPsi;
		this->element(0, 1) = (cosPhi * sinPsi) + (sinPhi * sinTheta * cosPsi);
		this->element(0, 2) = (sinPhi * sinPsi) - (cosPhi * sinTheta * cosPsi);

		this->element(1, 0) = -cosTheta * sinPsi;
		this->element(1, 1) = (cosPhi * cosPsi) - (sinPhi * sinTheta * sinPsi);
		this->element(1, 2) = (sinPhi * cosPsi) + (cosPhi * sinTheta * sinPsi);

		this->element(2, 0) = sinTheta;
		this->element(2, 1) = -sinPhi * cosTheta;
		this->element(2, 2) = cosPhi * cosTheta;
	}
	

	~rotation() = default;

	virtual GetType() { return transform::type::ROTATION; }
};


#endif // ! PROJECTION_H_