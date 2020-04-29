#ifndef VULKAN_HAL_H_
#define VULKAN_HAL_H_

#include "core/ehm/ehm.h"

// epoch Vulkan HAL
// epoch/src/hal/vulkan/VulkanHAL.h

#include "hal/hal.h"
#include <vulkan/vulkan.h>

class VulkanHAL :
	public HAL
{

public:
	VulkanHAL();

protected:
	virtual ~VulkanHAL() override;

public:
	virtual RESULT Initialize() override;

	virtual HAL::type GetType() override {
		return HAL::type::vulkan;
	}

private:
	RESULT EnumerateInstanceExtensions();

private:
	VkInstance m_vkInstance;

	uint32_t m_extensionCount = 0;
	EPVector<VkExtensionProperties> m_extensions;

};


#endif // ! VULKAN_HAL_H_