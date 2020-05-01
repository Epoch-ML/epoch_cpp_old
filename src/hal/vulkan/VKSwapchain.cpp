#include "VKSwapchain.h"

#include "VulkanUtilities.h"

#include <algorithm>

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

// This is split since this call can simply query the object
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

EPRef<VKSwapchain> VKSwapchain::make(
	VkPhysicalDevice vkPhysicalDevice, 
	VkSurfaceKHR vkSurface, 
	VkFormat vkSurfaceFormat, 
	VkColorSpaceKHR vkColorSpaceKHR,
	VkPresentModeKHR vkPresentModeKHR,
	VkExtent2D vkExtent2D
) {
	RESULT r = R::OK;
	EPRef<VKSwapchain> pVKSwapchain = nullptr;

	CNM(vkPhysicalDevice, "Cannot make vk swapchain without a valid physical device");
	CNM(vkSurface, "Cannot make vk swapchain without a valid surface");

	pVKSwapchain = VKSwapchain::make(vkPhysicalDevice, vkSurface);
	CNM(pVKSwapchain, "Failed to initialize VK swapchain");

	// Select valid presentation mode and format
	CRM(pVKSwapchain->SelectSurfaceFormat(vkSurfaceFormat, vkColorSpaceKHR),
		"Failed to select surface format");

	CRM(pVKSwapchain->SelectPresentationMode(vkPresentModeKHR),
		"Failed to select presentation mode");

	CRM(pVKSwapchain->SelectSwapchainExtent(vkExtent2D),
		"Failed to set swap chain extends");

	CRM(pVKSwapchain->CreateSwapchain(), "Failed to create swapchain");

Success:
	return pVKSwapchain;

Error:
	pVKSwapchain = nullptr;
	return nullptr;
}

RESULT VKSwapchain::SelectSurfaceFormat(VkFormat vkSurfaceFormat, VkColorSpaceKHR vkColorSpaceKHR) {
	RESULT r = R::OK;
	bool fFound = false;

	for (const auto& availableFormat : m_vkSurfaceFormats) {
		if (availableFormat.format == vkSurfaceFormat &&
			availableFormat.colorSpace == vkColorSpaceKHR)
		{
			m_vkSelectedSurfaceFormat = availableFormat;
			fFound = true;
			break;
		}
	}

	CBM(fFound, "Failed to find selected surface format");

Error:
	return r;
}

RESULT VKSwapchain::SelectPresentationMode(VkPresentModeKHR vkPresentModeKHR) {
	RESULT r = R::OK;
	bool fFound = false;

	for (const auto& availablePresentationMode : m_vkPresentationModes) {
		if (availablePresentationMode == vkPresentModeKHR) {
			m_vkSelectedPresentationMode = availablePresentationMode;
			fFound = true;
			break;
		}
	}

	CBM(fFound, "Failed to find selected presentation mode");

Error:
	return r;
}

RESULT VKSwapchain::SelectSwapchainExtent(VkExtent2D vkExtent2D) {
	RESULT r = R::OK;

	if (m_vkSurfaceCapabilities.currentExtent.width != UINT32_MAX) {
		m_vkSelectedExtent2D = m_vkSurfaceCapabilities.currentExtent;
	}
	else {
		m_vkSelectedExtent2D = vkExtent2D;

		m_vkSelectedExtent2D.width = std::max(
			m_vkSurfaceCapabilities.minImageExtent.width,
			std::min(m_vkSurfaceCapabilities.maxImageExtent.width, m_vkSelectedExtent2D.width));
		m_vkSelectedExtent2D.height = std::max(
			m_vkSurfaceCapabilities.minImageExtent.height,
			std::min(m_vkSurfaceCapabilities.maxImageExtent.height, m_vkSelectedExtent2D.height));
	}

	DEBUG_LINEOUT("vkExtends: {%d, %d}", m_vkSelectedExtent2D.width, m_vkSelectedExtent2D.height);

Error:
	return r;
}

RESULT VKSwapchain::CreateSwapchain() {
	RESULT r = R::OK;

	EPVector<uint32_t> familyQueueIndexes;

	CNM(m_vkPhysicalDevice, "Cannot create swapchain without a valid physical device");
	CNM(m_vkSurface, "Cannot create swapchain without a valid surface");

	m_swapchainImageCount = m_vkSurfaceCapabilities.minImageCount + 1;

	if (m_vkSurfaceCapabilities.maxImageCount > 0 && m_swapchainImageCount > m_vkSurfaceCapabilities.maxImageCount) {
		m_swapchainImageCount = m_vkSurfaceCapabilities.maxImageCount;
	}

	m_vkSwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	m_vkSwapchainCreateInfo.surface = m_vkSurface;
	m_vkSwapchainCreateInfo.minImageCount = m_swapchainImageCount;
	m_vkSwapchainCreateInfo.imageFormat = m_vkSelectedSurfaceFormat.format;
	m_vkSwapchainCreateInfo.imageColorSpace = m_vkSelectedSurfaceFormat.colorSpace;
	m_vkSwapchainCreateInfo.imageExtent = m_vkSelectedExtent2D;
	m_vkSwapchainCreateInfo.imageArrayLayers = 1;
	m_vkSwapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	familyQueueIndexes = FindQueueFamilies(m_vkPhysicalDevice, m_vkSurface);

	if (familyQueueIndexes.size() > 1) {
		m_vkSwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		m_vkSwapchainCreateInfo.queueFamilyIndexCount = 2;
		m_vkSwapchainCreateInfo.pQueueFamilyIndices = familyQueueIndexes.data();
	}
	else {
		m_vkSwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		m_vkSwapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
		m_vkSwapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
	}

Error:
	return r;
}