#include "VKSwapchain.h"

RESULT VKSwapchain::Initialize() {
	RESULT r = R::OK;

	CNM(m_pParentHAL, "VKSwapchain needs a valid parent HAL");

	// Get Surface Capabilities
	CVKRM(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_pParentHAL->GetPhysicalDevice(), m_pParentHAL->GetSurface(), &m_vkSurfaceCapabilities),
		"Failed to get Physical device surface capabilities");

	// Query supported surface formats
	CVKRM(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pParentHAL->GetPhysicalDevice(), m_pParentHAL->GetSurface(), &m_vkPhysicalDeviceSurfaceFormatCount, nullptr),
		"Failed to get physical device surface format count");

	CBM(m_vkPhysicalDeviceSurfaceFormatCount != 0, "Physical device has no surface formats");

	m_vkSurfaceFormats = EPVector<VkSurfaceFormatKHR>(m_vkPhysicalDeviceSurfaceFormatCount);
	CVKRM(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pParentHAL->GetPhysicalDevice(), m_pParentHAL->GetSurface(), &m_vkPhysicalDeviceSurfaceFormatCount, m_vkSurfaceFormats.data(m_vkPhysicalDeviceSurfaceFormatCount)),
		"Failed to get physical device surface formats");

	// Query supported presentation modes
	
	CVKRM(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pParentHAL->GetPhysicalDevice(), m_pParentHAL->GetSurface(), &m_vkPhysicalDevicePresenationModeCount, nullptr),
		"Failed to get physical device presentation mode count");

	CBM(m_vkPhysicalDeviceSurfaceFormatCount != 0, "Physical device has no presentation modes");

	m_vkPresentationModes = EPVector< VkPresentModeKHR>(m_vkPhysicalDevicePresenationModeCount);
	CVKRM(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pParentHAL->GetPhysicalDevice(), m_pParentHAL->GetSurface(), &m_vkPhysicalDevicePresenationModeCount, m_vkPresentationModes.data(m_vkPhysicalDevicePresenationModeCount)),
		"Failed to get physical device presentation modes");

	// TODO: This neeeds to be in a query-able format

Error:
	return r;
}