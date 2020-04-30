#ifndef VULKAN_SWAPCHAIN_H_
#define VULKAN_SWAPCHAIN_H_

#include "core/ehm/ehm.h"

// epoch Vulkan HAL
// epoch/src/hal/vulkan/VulkanHAL.h

#include "hal/swapchain.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"

class VKSwapchain :
	public swapchain
{
private:
	VKSwapchain(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface) :
		m_vkPhysicalDevice(vkPhysicalDevice),
		m_vkSurface(vkSurface)
	{
		//
	}

	virtual RESULT Initialize() override;

public:
	virtual ~VKSwapchain() override = default;

	static EPRef<VKSwapchain> make(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface);

	const EPVector<VkSurfaceFormatKHR>& SurfaceFormats() const { 
		return const_cast<const EPVector<VkSurfaceFormatKHR>&>(m_vkSurfaceFormats); 
	}

	const EPVector<VkPresentModeKHR>& PresentationModes() const { 
		return const_cast<const EPVector<VkPresentModeKHR>&>(m_vkPresentationModes);
	}

	RESULT SelectSurfaceFormat(VkFormat, VkColorSpaceKHR);
	RESULT SelectPresentationMode(VkPresentModeKHR);

private:
	VkSurfaceCapabilitiesKHR m_vkSurfaceCapabilities = {};
	uint32_t m_vkPhysicalDeviceSurfaceFormatCount = 0;
	EPVector<VkSurfaceFormatKHR> m_vkSurfaceFormats;
	VkSurfaceFormatKHR m_vkSelectedSurfaceFormat = {};

	uint32_t m_vkPhysicalDevicePresenationModeCount = 0;
	EPVector<VkPresentModeKHR> m_vkPresentationModes;
	VkPresentModeKHR m_vkSelectedPresentationMode = {};

	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkSurfaceKHR m_vkSurface = nullptr;

};

#endif // ! VULKAN_SWAPCHAIN_H_