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
// Instance Extensions
	const EPVector<EPTuple<int, const char*>> m_RequiredExtensions = {
		{VK_KHR_surface, VK_KHR_SURFACE_EXTENSION_NAME}
	};

	RESULT EnumerateInstanceExtensions();

// Validation Layers
	const EPVector<const char*> m_vkRequiredValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	RESULT EnumerateValidationLayers();


private:
	VkInstance m_vkInstance;
	VkApplicationInfo m_vkApplicationInfo = {};
	VkInstanceCreateInfo m_vkInstanceCreateInfo = {};

// Instance Extensions
	uint32_t m_vkEnumeratedExtensionCount = 0;
	EPVector<VkExtensionProperties> m_vkEnumeratedExtensions;

	EPVector<VkExtensionProperties> m_vkExtensions;
	const char* m_vkExtensionNames[64] = { 0 };		// TODO: not a huge fan of the duplication here

// Validation Layers
	uint32_t m_vkValidationLayerCount;
	EPVector<VkLayerProperties> m_vkAvailableValidationLayers;

#ifdef NDEBUG
	const bool m_fEnableValidationLayers = false;
#else
	const bool m_fEnableValidationLayers = true;
#endif
};


#endif // ! VULKAN_HAL_H_