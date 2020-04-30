#include "VKSwapchain.h"

#include "VulkanUtilities.h"


RESULT VKSwapchain::Initialize() {
	RESULT r = R::OK;

	CNM(m_vkPhysicalDevice, "VKSwapchain needs a valid physical device");
	CNM(m_vkSurface, "VKSwapchain needs a valid surface");

	// Get Surface Capabilities
	CVKRM(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_vkPhysicalDevice, m_vkSurface, &m_vkSurfaceCapabilities),
		"Failed to get Physical device surface capabilities");

	// Query supported surface formats
	CVKRM(vkGetPhysicalDeviceSurfaceFormatsKHR(m_vkPhysicalDevice, m_vkSurface, &m_vkPhysicalDeviceSurfaceFormatCount, nullptr),
		"Failed to get physical device surface format count");

	CBM(m_vkPhysicalDeviceSurfaceFormatCount != 0, "Physical device has no surface formats");

	m_vkSurfaceFormats = EPVector<VkSurfaceFormatKHR>(m_vkPhysicalDeviceSurfaceFormatCount);
	CVKRM(vkGetPhysicalDeviceSurfaceFormatsKHR(m_vkPhysicalDevice, m_vkSurface, &m_vkPhysicalDeviceSurfaceFormatCount, m_vkSurfaceFormats.data(m_vkPhysicalDeviceSurfaceFormatCount)),
		"Failed to get physical device surface formats");

	// Query supported presentation modes
	
	CVKRM(vkGetPhysicalDeviceSurfacePresentModesKHR(m_vkPhysicalDevice, m_vkSurface, &m_vkPhysicalDevicePresenationModeCount, nullptr),
		"Failed to get physical device presentation mode count");

	CBM(m_vkPhysicalDeviceSurfaceFormatCount != 0, "Physical device has no presentation modes");

	m_vkPresentationModes = EPVector< VkPresentModeKHR>(m_vkPhysicalDevicePresenationModeCount);
	CVKRM(vkGetPhysicalDeviceSurfacePresentModesKHR(m_vkPhysicalDevice, m_vkSurface, &m_vkPhysicalDevicePresenationModeCount, m_vkPresentationModes.data(m_vkPhysicalDevicePresenationModeCount)),
		"Failed to get physical device presentation modes");

Error:
	return r;
}

EPRef<VKSwapchain> VKSwapchain::make(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface) {
	RESULT r = R::OK;
	EPRef<VKSwapchain> pVKSwapchain = nullptr;

	CNM(vkPhysicalDevice, "Cannot make vk swapchain without a valid physical device");
	CNM(vkSurface, "Cannot make vk swapchain without a valid surface");

	pVKSwapchain = new VKSwapchain(vkPhysicalDevice, vkSurface);
	CNM(pVKSwapchain, "Failed to allocate swapchain");

	CRM(pVKSwapchain->Initialize(), "Failed to initialize VK swapchain");

Success:
	return pVKSwapchain;

Error:
	pVKSwapchain = nullptr;
	return nullptr;
}