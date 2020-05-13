#ifndef VIEW_H_
#define VIEW_H_

#include "core/ehm/result.h"

// epoch math matrix view
// epoch/src/core/math/matrix/view.h

// View matrix

#include "core/math/matrix/transform.h"

#include "core/math/vector.h"
#include "core/math/math.h"

#include "core/math/point.h"
#include "core/math/vector.h"

template <typename TValue = float>
class view :
	public transform<TValue>
{
public:
	view() {
		this->SetIdentity(1.0f);
	}

	~view() = default;


	static view<TValue> MakeLookAtViewMatrix(point<TValue> ptEye, point<TValue> ptLookAt, vector<TValue> vUp) {
		
		view<TValue> matView;

		vector<TValue> vView = (ptEye - ptLookAt).normal();
		vector<TValue> vLeft = vView.cross(vUp);
		vector<TValue> vUpDirection = vLeft.cross(vView);

		//vView.invert();

		// View is essentially basis * translation of -ptEye

		// TODO: replace with basis matrix?
		matView.element(0, 0) = vLeft.x();
		matView.element(0, 1) = vLeft.y();
		matView.element(0, 2) = vLeft.z();
		matView.element(0, 3) = -1.0f * vLeft.dot(ptEye);

		matView.element(1, 0) = vUpDirection.x();
		matView.element(1, 1) = vUpDirection.y();
		matView.element(1, 2) = vUpDirection.z();
		matView.element(1, 3) = -1.0f * vUpDirection.dot(ptEye);

		matView.element(2, 0) = vView.x();
		matView.element(2, 1) = vView.y();
		matView.element(2, 2) = vView.z();
		matView.element(2, 3) = -1.0f * vView.dot(ptEye);

		return matView;
	}
};

#endif // ! VIEW_H_