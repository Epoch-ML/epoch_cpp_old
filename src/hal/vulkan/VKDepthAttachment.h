#ifndef VULKAN_DEPTH_ATTACHMENT_H_
#define VULKAN_DEPTH_ATTACHMENT_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Depth Attachment
// epoch/src/hal/vulkan/VKDepthAttachment.h

#include "hal/attachment.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

class VKImage;
class VKImageView;
class VKSwapchain;
class VKCommandPool;

class VKDepthAttachment :
	public attachment,
	public EPFactoryMethod<VKDepthAttachment, VkPhysicalDevice, VkDevice, EPRef<VKSwapchain>, const EPRef<VKCommandPool>&>
{
private:
	VKDepthAttachment(
		VkPhysicalDevice vkPhysicalDevice, 
		VkDevice vkLogicalDevice, 
		EPRef<VKSwapchain> pVKSwapchain, 
		const EPRef<VKCommandPool>& pVKCommandPool);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

	VkFormat FindDepthFormat();

public:
	virtual ~VKDepthAttachment() override;

	static EPRef<VKDepthAttachment> InternalMake(VkPhysicalDevice, VkDevice, EPRef<VKSwapchain>, const EPRef<VKCommandPool>&);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;
	EPRef<VKCommandPool> m_pVKCommandPool = nullptr;
	
	VkFormat m_vkDepthFormat = VK_FORMAT_UNDEFINED;

	EPRef<VKImage> m_pVKImage = nullptr;
	EPRef<VKImageView> m_pVKImageView = nullptr;
};

#endif // ! VULKAN_DEPTH_ATTACHMENT_H_