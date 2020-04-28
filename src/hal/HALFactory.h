#ifndef HAL_FACTORY_H_
#define HAL_FACTORY_H_

#include "core/ehm/ehm.h"

// epoch hal factory
// epoch/src/sandbox/HALFactory.h

# include "core/types/EPFactory.h"

#include "sandbox/sandbox.h"
#include "sandbox/SandboxProcess.h"
#include "core/types/EPRef.h"

#include "hal/hal.h"

class HALFactory :
	public EPFactory<HALFactory, HAL, HAL::type, EPRef<SandboxProcess>>
{
	friend EPFactory;

public:
	HALFactory() {
		//
	}

	virtual ~HALFactory() override {
		//
	}

protected:
	static EPRef<HAL> InternalMake(HAL::type halType, EPRef<SandboxProcess> pSBProcess);

private:
	static EPRef<HAL> InternalMakeVulkan(EPRef<SandboxProcess> pSBProcess);
};

#endif // ! HAL_FACTORY_H_