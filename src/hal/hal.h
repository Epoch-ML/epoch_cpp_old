#ifndef HAL_H_
#define HAL_H_

#include "core/ehm/ehm.h"

// epoch HAL (Hardware Abstraction Library)
// epoch/src/hal/hal.h

#include "core/types/EPObj.h"

#include "core/types/EPProcess.h"
#include "core/types/EPString.h"
#include "core/types/EPRef.h"

class SandboxProcess;

class HAL : 
	public EPObj
{
public:
	enum class type : uint32_t {
		vulkan,
		custom
	};
	static const char* kHALTypes[];

	static const char* GetTypeName(HAL::type halType) {
		return kHALTypes[static_cast<uint8_t>(halType)];
	}

public:
	HAL();
	virtual ~HAL() override;

	virtual HAL::type GetType() = 0;
	virtual RESULT Initialize(const EPRef<SandboxProcess>&) = 0;


private:
	// 
};

#endif // ! HAL_H_