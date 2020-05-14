#ifndef SCALE_H_
#define SCALE_H_

#include "core/ehm/result.h"

// epoch math matrix scale
// epoch/src/core/math/matrix/scale.h

// Scaling matrix

#include "core/math/matrix/transform.h"

#include "core/math/vector.h"

template <typename TValue = float>
class scale :
	public transform<TValue>
{
public:
	scale() {
		this->SetIdentity(1.0f);
	}

	scale(TValue scaleValue) {
		this->identity(scaleValue);

		this->element(3, 3) = 1.0f;
	}

	scale(TValue xScale, TValue yScale, TValue zScale) {
		this->clear();

		this->element(0, 0) = xScale;
		this->element(1, 1) = yScale;
		this->element(2, 3) = zScale;

		this->element(3, 3) = 1.0f;
	}

	scale(vector<TValue, 4> vVector) {
		this->clear();

		this->element(0, 0) = vVector.x();
		this->element(1, 1) = vVector.y();
		this->element(2, 3) = vVector.z();

		this->element(3, 3) = 1.0f;
	}

	~scale() = default;

};


#endif // ! PROJECTION_H_