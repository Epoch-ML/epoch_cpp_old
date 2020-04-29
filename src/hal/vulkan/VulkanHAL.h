#ifndef VULKAN_HAL_H_
#define VULKAN_HAL_H_

#include "core/ehm/ehm.h"

// epoch Vulkan HAL
// epoch/src/hal/vulkan/VulkanHAL.h

#include "hal/hal.h"
#include <vulkan/vulkan.h>

#include "core/types/EPTuple.h"

class VulkanHAL :
	public HAL
{

public:
	VulkanHAL();

protected:
	virtual ~VulkanHAL() override;

public:
	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

	virtual HAL::type GetType() override {
		return HAL::type::vulkan;
	}

private:
	static EPTuple<int, const char*> k_requiredVKExtensions[];

	RESULT EnumerateInstanceExtensions();
	RESULT AddRequiredInstanceExtensions();

private:
	VkInstance m_vkInstance;

	uint32_t m_vkEnumeratedExtensionCount = 0;
	EPVector<VkExtensionProperties> m_vkEnumeratedExtensions;

	EPVector<VkExtensionProperties> m_vkExtensions;
	const char* m_vkExtensionNames[64] = { 0 };		// TODO: not a huge fan of the duplication here

	VkApplicationInfo m_vkApplicationInfo = {};
	VkInstanceCreateInfo m_vkInstanceCreateInfo = {};
};


#endif // ! VULKAN_HAL_H_