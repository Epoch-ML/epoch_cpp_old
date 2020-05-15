#ifndef VULKAN_IMAGE_H_
#define VULKAN_IMAGE_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Image
// epoch/src/hal/vulkan/VKImage.h

#include <vulkan/vulkan.h>

#include "VulkanUtilities.h"

#include "hal/image.h"

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

class VKImage :
	public image,
	public EPFactoryMethod<VKImage, VkPhysicalDevice, VkDevice, uint32_t, uint32_t>
{
private:
	VKImage(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice, uint32_t width, uint32_t height);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKImage() override;
	static EPRef<VKImage> InternalMake(VkPhysicalDevice, VkDevice, uint32_t, uint32_t);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	VkImage m_vkTextureImage = nullptr;
	VkDeviceMemory m_vkTextureDeviceMemory = nullptr;
};

#endif // ! VULKAN_IMAGE_H_