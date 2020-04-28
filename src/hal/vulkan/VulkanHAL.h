#ifndef VULKAN_HAL_H_
#define VULKAN_HAL_H_

#include "core/ehm/ehm.h"

// epoch Vulkan HAL
// epoch/src/hal/vulkan/VulkanHAL.h

#include "hal/hal.h"

class VulkanHAL :
	public HAL
{

public:
	VulkanHAL();

protected:
	virtual ~VulkanHAL() override;

public:
	virtual RESULT Initialize(const EPRef<SandboxProcess>& pSBProcess) override;

	virtual HAL::type GetType() override {
		return HAL::type::vulkan;
	}

private:

};


#endif // ! VULKAN_HAL_H_