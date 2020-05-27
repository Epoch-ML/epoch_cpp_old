#ifndef VULKAN_UTILITIES_H_
#define VULKAN_UTILITIES_H_

#include "core/ehm/ehm.h"

// epoch vulkan utilities
// epoch/src/hal/vulkan/VulkanUtilities.h

#include <vulkan/vulkan.h>

#include "core/types/EPTuple.h"
#include "core/types/EPVector.h"

// Extend the EHM for VK a bit

#define CVKR(vkr) CR((RESULT)(vkr))
#define CVKRM(vkr, msg, ...) CRM((RESULT)(vkr), msg, ##__VA_ARGS__)

const char* VkErrorString(VkResult vkr);

// TODO: move this to an 'extensions' object or namespace?
RESULT CreateDebugUtilsMessengerEXT(
	VkInstance vkInstance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pVKAllocationCallbacks,
	VkDebugUtilsMessengerEXT* pVKDebugMessenger);

RESULT DestroyDebugUtilsMessengerEXT(
	VkInstance vkInstance,
	VkDebugUtilsMessengerEXT vkDebugMessenger,
	const VkAllocationCallbacks* pVkAllocationCallbacks);

EPVector<VkQueueFamilyProperties> EnumerateVKPhysicalDeviceQueueFamilies(VkPhysicalDevice vkPhysicalDevice);

VkFormat FindSupportedFormat(
	VkPhysicalDevice vkPhysicalDevice,
	const EPVector<VkFormat>& vkFormatCandidates, 
	VkImageTiling vkImageTiling, 
	VkFormatFeatureFlags vkFormatFeatureFlags);

bool VKFormatHasStencilComponent(VkFormat vkFormat);

class VKQueueFamilies {
public:
	VKQueueFamilies() = default;

	bool HasGraphicsQueue() { return fGraphicsQueue; }
	bool HasComputeQueue() { return fComputeQueue; }
	bool HasPresentationQueue() { return fPresentationQueue; }
	bool HasTransferQueue() { return fTransferQueue; }

	uint32_t GetGraphicsQueueIndex() { return graphicsQueueIndex; }
	uint32_t GetComputeQueueIndex() { return computeQueueIndex; }
	uint32_t GetPresentationQueueIndex() { return presentationQueueIndex; }
	uint32_t GetTransferQueueIndex() { return transferQueueIndex; }

	void SetGraphicsQueueIndex(uint32_t index) { 
		fGraphicsQueue = true;
		graphicsQueueIndex = index; 
	}
	void SetComputeQueueIndex(uint32_t index) {
		fComputeQueue = true;
		computeQueueIndex = index;
	}
	void SetPresentationQueueIndex(uint32_t index) {
		fPresentationQueue = true;
		presentationQueueIndex = index;
	}
	void SetTransferQueueIndex(uint32_t index) {
		fTransferQueue = true;
		transferQueueIndex = index;
	}

	EPVector<uint32_t> GetUniqueIndexes() {
		EPVector<uint32_t> retVector;
		
		retVector.PushBackUnique(GetGraphicsQueueIndex());
		retVector.PushBackUnique(GetComputeQueueIndex());
		retVector.PushBackUnique(GetPresentationQueueIndex());
		retVector.PushBackUnique(GetTransferQueueIndex());

		return retVector;
	}

private:
	// TODO: replace with tuple once fixed
	bool fGraphicsQueue = false;
	uint32_t graphicsQueueIndex = 0;

	bool fComputeQueue = false;
	uint32_t computeQueueIndex = 0;

	bool fPresentationQueue = false;
	uint32_t presentationQueueIndex = 0;

	bool fTransferQueue = false;
	uint32_t transferQueueIndex = 0;
};

VKQueueFamilies FindQueueFamilies(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface);


#endif // !VULKAN_UTILITIES_H_