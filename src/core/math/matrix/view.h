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
		this->identity(1.0f);
	}

	~view() = default;

	virtual GetType() { return transform::type::VIEW; }

	static view<TValue> MakeLookAtViewMatrix(point<TValue> ptEye, point<TValue> ptLookAt, vector<TValue> vUp) {
		
		view<TValue> matView;

		vector<TValue> vView = (ptEye - ptLookAt).normalize();
		vector<TValue> vLeft = vector::cross(vView, vUp);
		vector<TValue> vUp = vector::cross(vLeft, vView);

		vView.invert();

		// View is essentially basis * translation of -ptEye

		// TODO: replace with basis matrix?
		matView.element(0, 0) = vLeft.x();
		matView.element(0, 1) = vLeft.y();
		matView.element(0, 2) = vLeft.z();
		matView.element(0, 3) = -1.0f * vector::dot(vLeft, ptEye);

		matView.element(1, 0) = vUp.x();
		matView.element(1, 1) = vUp.y();
		matView.element(1, 2) = vUp.z();
		matView.element(1, 3) = -1.0f * vector::dot(vUp, ptEye);

		matView.element(1, 0) = vView.x();
		matView.element(1, 1) = vView.y();
		matView.element(1, 2) = vView.z();
		matView.element(1, 3) = -1.0f * vector::dot(vView, ptEye);

		return matView;
	}
};


#endif // ! VIEW_H_