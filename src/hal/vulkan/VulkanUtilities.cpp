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

VKQueueFamilies FindQueueFamilies(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface) {
	RESULT r = RESULT::OK;

	VKQueueFamilies vkQueueFamilies;

	EPVector<VkQueueFamilyProperties> vkQueueFamilyProperties;

	uint32_t familyQueueIndex = 0;

	bool fFoundQueueFamily = false;
	EPVector<uint32_t> familyQueueIndexes;

	CNM(vkPhysicalDevice, "Cannot initialize logical device without a valid physical device");
	CNM(vkSurface, "Cannot initialize logical device without a valid surface");

	vkQueueFamilyProperties = EnumerateVKPhysicalDeviceQueueFamilies(vkPhysicalDevice);
	CBM(vkQueueFamilyProperties.size() != 0, "Failed to enumerate physical device queue families");

	for (auto& family : vkQueueFamilyProperties) {
		bool fGraphicsOrCompute = false;

		// Graphics Queue
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			fGraphicsOrCompute = true;
			if (vkQueueFamilies.HasGraphicsQueue() == false) {
				vkQueueFamilies.SetGraphicsQueueIndex(familyQueueIndex);
			}
		}
		
		// Compute Queue
		if (family.queueFlags & VK_QUEUE_COMPUTE_BIT) {
			fGraphicsOrCompute = true;
			if (vkQueueFamilies.HasComputeQueue() == false) {
				vkQueueFamilies.SetComputeQueueIndex(familyQueueIndex);
			}
		}

		// Transfer Queue
		if (fGraphicsOrCompute == false && family.queueFlags & VK_QUEUE_TRANSFER_BIT) {
			if (vkQueueFamilies.HasTransferQueue() == false) {
				vkQueueFamilies.SetTransferQueueIndex(familyQueueIndex);
			}
		}

		// Presentation Queue
		if (vkQueueFamilies.HasGraphicsQueue()) {
			VkBool32 fPresentationSupport = false;
			CVKRM(vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, vkQueueFamilies.GetGraphicsQueueIndex(), vkSurface, &fPresentationSupport),
				"Failed to get PhysicalDeviceSurfaceSupport");

			if (fPresentationSupport == (VkBool32)(true)) {
				if (vkQueueFamilies.HasPresentationQueue() == false) {
					vkQueueFamilies.SetPresentationQueueIndex(familyQueueIndex);
				}
			}
		}

		familyQueueIndex++;
	}

Error:
	return vkQueueFamilies;
}

VkFormat FindSupportedFormat(
	VkPhysicalDevice vkPhysicalDevice,
	const EPVector<VkFormat>& vkFormatCandidates,
	VkImageTiling vkImageTiling,
	VkFormatFeatureFlags vkFormatFeatureFlags)
{
	for (auto& vkFormat : vkFormatCandidates) {
		VkFormatProperties vkFormatProperties; 
		vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, vkFormat, &vkFormatProperties);

		if ((vkImageTiling == VK_IMAGE_TILING_LINEAR) &&
			(vkFormatProperties.linearTilingFeatures & vkFormatFeatureFlags) == vkFormatFeatureFlags)
		{
			return vkFormat;
		}
		else if ((vkImageTiling == VK_IMAGE_TILING_OPTIMAL) &&
				 (vkFormatProperties.optimalTilingFeatures & vkFormatFeatureFlags) == vkFormatFeatureFlags) 
		{
			return vkFormat;
		}
	}

	// not found
	return VK_FORMAT_UNDEFINED;
}

bool VKFormatHasStencilComponent(VkFormat vkFormat) {
	return (vkFormat == VK_FORMAT_D32_SFLOAT_S8_UINT) || (vkFormat == VK_FORMAT_D24_UNORM_S8_UINT);
}