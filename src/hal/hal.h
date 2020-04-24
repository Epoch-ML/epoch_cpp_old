#ifndef HAL_H_
#define HAL_H_

#include "core/ehm/ehm.h"

// epoch HAL (Hardware Abstraction Library)
// epoch/src/hal/hal.h

#include "core/types/EPObj.h"

#include "core/types/EPProcess.h"
#include "core/types/EPString.h"

class HAL : 
	public EPObj
{
public:
	enum class type : uint32_t {
		vulkan,
		custom
	};
	static const char* kHALTypes[];

public:
	HAL();
	virtual ~HAL() override;

	virtual HAL::type GetType() = 0;
	virtual RESULT Initialize() = 0;

private:
	// 
};

#endif // ! HAL_H_