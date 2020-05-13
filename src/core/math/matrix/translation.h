#ifndef TRANSLATION_H_
#define TRANSLATION_H_

#include "core/ehm/result.h"

// epoch math matrix translation
// epoch/src/core/math/matrix/translation.h

// Translation matrix

#include "core/math/matrix/transform.h"

#include "core/math/vector.h"

template <typename TValue = float>
class translation :
	public transform<TValue>
{
public:
	translation() {
		this->SetIdentity(1.0f);
	}

	translation(TValue xTranslate, TValue yTranslate, TValue zTranslate) {
		this->clear();

		this->element(0, 3) = xTranslate;
		this->element(1, 3) = yTranslate;
		this->element(2, 3) = zTranslate;

		this->element(3, 3) = 1.0f;
	}

	translation(vector<TValue, 4> vVector) {
		this->clear();

		this->element(0, 3) = vVector.x();
		this->element(1, 3) = vVector.y();
		this->element(2, 3) = vVector.z();

		this->element(3, 3) = 1.0f;
	}

	~translation() = default;

};


#endif // ! PROJECTION_H_