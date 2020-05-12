#ifndef MATH_H_
#define MATH_H_

#include "core/ehm/result.h"

// epoch math
// epoch src/core/math/math.h

// A common math utilities file under math namespace

#define RIGHT_HANDED
#ifdef RIGHT_HANDED
	#define ROTATION_HAND_SIGN 1
#elif defined(LEFT_HANDED)
	#define ROTATION_HAND_SIGN -1
#endif

namespace math {

	enum class axis : uint8_t {
		X,
		Y,
		Z,
		XY,
		XZ,
		YZ,
		XYZ
	};

}

#endif // ! MATH_H_