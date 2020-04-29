#ifndef VULKAN_UTILITIES_H_
#define VULKAN_UTILITIES_H_

#include "core/ehm/ehm.h"

// epoch vulkan utilities
// epoch/src/hal/vulkan/VulkanUtilities.h

#include <vulkan/vulkan.h>

// Extend the EHM for VK a bit

#define CVKR(vkr) CR((RESULT)(vkr))
#define CVKRM(vkr, msg, ...) CRM((RESULT)(vkr), msg, ##__VA_ARGS__)

const char* VkErrorString(VkResult vkr);

#endif // !VULKAN_UTILITIES_H_