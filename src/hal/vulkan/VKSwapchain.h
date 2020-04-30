#ifndef VULKAN_SWAPCHAIN_H_
#define VULKAN_SWAPCHAIN_H_

#include "core/ehm/ehm.h"

// epoch Vulkan HAL
// epoch/src/hal/vulkan/VulkanHAL.h

#include "hal/swapchain.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"

class VKSwapchain :
	public swapchain
{
public:
	VKSwapchain(VulkanHAL* pParentHAL) :
		m_pParentHAL(pParentHAL)
	{
		//
	}

	virtual VKSwapchain override = default;

	virtual RESULT Initialize() override;

private:
	VkSurfaceCapabilitiesKHR m_vkSurfaceCapabilities = {};
	EPVector<VkSurfaceFormatKHR> m_vkSurfaceFormats;
	EPVector<VkPresentModeKHR> m_vkPresentationModes;

	VulkanHAL* m_pParentHAL = nullptr;
};

#endif // ! VULKAN_SWAPCHAIN_H_