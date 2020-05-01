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

	RESULT SelectSurfaceFormat(VkFormat, VkColorSpaceKHR);
	RESULT SelectPresentationMode(VkPresentModeKHR);
	RESULT SelectSwapchainExtent(VkExtent2D vkExtent2D);
	RESULT CreateSwapchain();


public:
	virtual ~VKSwapchain() override = default;

	// This will actually create the swapchain
	static EPRef<VKSwapchain> make(
		VkPhysicalDevice vkPhysicalDevice,
		VkSurfaceKHR vkSurface,
		VkFormat vkSurfaceFormat,
		VkColorSpaceKHR vkColorSpaceKHR,
		VkPresentModeKHR vkPresentModeKHR,
		VkExtent2D vkExtent2D
	);

	// This just gets the swap chain initialized
	static EPRef<VKSwapchain> make(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface);

	const EPVector<VkSurfaceFormatKHR>& SurfaceFormats() const { 
		return const_cast<const EPVector<VkSurfaceFormatKHR>&>(m_vkSurfaceFormats); 
	}

	const EPVector<VkPresentModeKHR>& PresentationModes() const { 
		return const_cast<const EPVector<VkPresentModeKHR>&>(m_vkPresentationModes);
	}

	

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

	VkExtent2D m_vkSelectedExtent2D;
	uint32_t m_swapchainImageCount = 0;

	VkSwapchainCreateInfoKHR m_vkSwapchainCreateInfo = {};

};

#endif // ! VULKAN_SWAPCHAIN_H_