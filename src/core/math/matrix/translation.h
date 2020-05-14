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
		this->SetIdentity(1.0f);

		this->element(0, 3) = xTranslate;
		this->element(1, 3) = yTranslate;
		this->element(2, 3) = zTranslate;
	}

	translation(matrix<TValue, 4, 1> vVector) {
		this->SetIdentity(1.0f);

		this->element(0, 3) = vVector(0);
		this->element(1, 3) = vVector(1);
		this->element(2, 3) = vVector(2);
	}

	~translation() = default;

};


#endif // ! PROJECTION_H_