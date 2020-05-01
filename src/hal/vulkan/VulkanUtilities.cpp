#include "VulkanUtilities.h"


const char* VkErrorString(VkResult vkr) {
	switch (vkr) {
#define VK_STR_FMT(res) case VK_ ##res: return #res
		VK_STR_FMT(NOT_READY);
		VK_STR_FMT(TIMEOUT);
		VK_STR_FMT(EVENT_SET);
		VK_STR_FMT(EVENT_RESET);
		VK_STR_FMT(INCOMPLETE);
		VK_STR_FMT(ERROR_OUT_OF_HOST_MEMORY);
		VK_STR_FMT(ERROR_OUT_OF_DEVICE_MEMORY);
		VK_STR_FMT(ERROR_INITIALIZATION_FAILED);
		VK_STR_FMT(ERROR_DEVICE_LOST);
		VK_STR_FMT(ERROR_MEMORY_MAP_FAILED);
		VK_STR_FMT(ERROR_LAYER_NOT_PRESENT);
		VK_STR_FMT(ERROR_EXTENSION_NOT_PRESENT);
		VK_STR_FMT(ERROR_FEATURE_NOT_PRESENT);
		VK_STR_FMT(ERROR_INCOMPATIBLE_DRIVER);
		VK_STR_FMT(ERROR_TOO_MANY_OBJECTS);
		VK_STR_FMT(ERROR_FORMAT_NOT_SUPPORTED);
		VK_STR_FMT(ERROR_SURFACE_LOST_KHR);
		VK_STR_FMT(ERROR_NATIVE_WINDOW_IN_USE_KHR);
		VK_STR_FMT(SUBOPTIMAL_KHR);
		VK_STR_FMT(ERROR_OUT_OF_DATE_KHR);
		VK_STR_FMT(ERROR_INCOMPATIBLE_DISPLAY_KHR);
		VK_STR_FMT(ERROR_VALIDATION_FAILED_EXT);
		VK_STR_FMT(ERROR_INVALID_SHADER_NV);
#undef VK_STR_FMT
	default:
		return "VK_UNKNOWN_ERROR";
	}
}

RESULT CreateDebugUtilsMessengerEXT(
	VkInstance vkInstance, 
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
	const VkAllocationCallbacks* pVKAllocationCallbacks, 
	VkDebugUtilsMessengerEXT* pVKDebugMessenger) 
{
	RESULT r = R::OK;

	auto pfnVKCreateDebugUtilsMessengerEXT = 
		(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance, "vkCreateDebugUtilsMessengerEXT");

	CNRM(pfnVKCreateDebugUtilsMessengerEXT, (RESULT)VK_ERROR_EXTENSION_NOT_PRESENT, 
		"PFN_vkCreateDebugUtilsMessengerEXT doesn't exist");

	CVKRM(pfnVKCreateDebugUtilsMessengerEXT(vkInstance, pCreateInfo, pVKAllocationCallbacks, pVKDebugMessenger), 
		"VKCreateDebugUtilsMessengerEXT failed");

Error:
	return r;
}

RESULT DestroyDebugUtilsMessengerEXT(
	VkInstance vkInstance, 
	VkDebugUtilsMessengerEXT vkDebugMessenger, 
	const VkAllocationCallbacks* pVkAllocationCallbacks) 
{
	RESULT r = R::OK;

	auto pfnVKDestroyDebugUtilsMessengerEXT = 
		(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInstance, "vkDestroyDebugUtilsMessengerEXT");

	CNRM(pfnVKDestroyDebugUtilsMessengerEXT, (RESULT)VK_ERROR_EXTENSION_NOT_PRESENT,
		"PFN_vkDestroyDebugUtilsMessengerEXT doesn't exist");

	pfnVKDestroyDebugUtilsMessengerEXT(vkInstance, vkDebugMessenger, pVkAllocationCallbacks);

Error:
	return r;
}

EPVector<VkQueueFamilyProperties> EnumerateVKPhysicalDeviceQueueFamilies(VkPhysicalDevice vkPhysicalDevice) {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;
	uint32_t queueFamilyCount = 0;
	EPVector<VkQueueFamilyProperties> vkPhysicalDeviceQueueFamilies;

	CNM(vkPhysicalDevice, "Cannot enumerate queue families without a valid physical device");

	vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);

	CBM(queueFamilyCount != 0, "Failed to find any queue families");

	vkPhysicalDeviceQueueFamilies = EPVector<VkQueueFamilyProperties>(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(
		vkPhysicalDevice,
		&queueFamilyCount,
		vkPhysicalDeviceQueueFamilies.data(queueFamilyCount)
	);

Error:
	return vkPhysicalDeviceQueueFamilies;
}

EPVector<uint32_t> FindQueueFamilies(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface) {
	RESULT r = RESULT::OK;

	EPVector<VkQueueFamilyProperties> vkQueueFamilyProperties;

	uint32_t familyQueueIndex = 0;
	int32_t graphicsFamilyQueueIndex = -1;
	int32_t presentFamilyQueueIndex = -1;
	bool fFoundQueueFamily = false;
	EPVector<uint32_t> familyQueueIndexes;

	CNM(vkPhysicalDevice, "Cannot initialize logical device without a valid physical device");
	CNM(vkSurface, "Cannot initialize logical device without a valid surface");

	vkQueueFamilyProperties = EnumerateVKPhysicalDeviceQueueFamilies(vkPhysicalDevice);
	CBM(vkQueueFamilyProperties.size() != 0, "Failed to enumerate physical device queue families");

	for (auto& family : vkQueueFamilyProperties) {
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			if (graphicsFamilyQueueIndex == -1)
				graphicsFamilyQueueIndex = familyQueueIndex;
		}

		VkBool32 fPresentationSupport = false;
		CVKRM(vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, graphicsFamilyQueueIndex, vkSurface, &fPresentationSupport),
			"Failed to get PhysicalDeviceSurfaceSupport");

		if (fPresentationSupport == (VkBool32)(true)) {
			if (presentFamilyQueueIndex == -1)
				presentFamilyQueueIndex = familyQueueIndex;
		}

		familyQueueIndex++;
	}

	CBM(graphicsFamilyQueueIndex >= 0, "Failed to find graphics queue");
	CBM(presentFamilyQueueIndex >= 0, "Failed to find surface presentation queue");

	// TODO: this should be more dynamic (above in particular)
	familyQueueIndexes.PushBack((uint32_t)graphicsFamilyQueueIndex);

	if (graphicsFamilyQueueIndex != presentFamilyQueueIndex)
		familyQueueIndexes.PushBack((uint32_t)presentFamilyQueueIndex);

Success:
	return familyQueueIndexes;

Error:
	return {};
}