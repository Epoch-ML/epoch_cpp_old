#ifndef VULKAN_TEXTURE_H_
#define VULKAN_TEXTURE_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Texture
// epoch/src/hal/vulkan/VKTexture.h

#include "hal/texture.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

class VKImage;

class VKTexture :
	public texture,
	public EPFactoryMethod<VKTexture, VkPhysicalDevice, VkDevice, const EPString<char>&>
{
private:
	VKTexture(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice, const EPString<char>&);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKTexture() override;
	static EPRef<VKTexture> InternalMake(VkPhysicalDevice, VkDevice, const EPString<char>&);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	
	VkBuffer m_vkStagingBuffer = nullptr;
	VkDeviceMemory m_vkStagingBufferDeviceMemory = nullptr;

	EPRef<VKImage> m_pVKImage = nullptr;
};

#endif // ! VULKAN_COMMAND_POOL_H_