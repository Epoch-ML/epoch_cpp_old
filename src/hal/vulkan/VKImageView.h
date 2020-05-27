#ifndef VULKAN_IMAGE_VIEW_H_
#define VULKAN_IMAGE_VIEW_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Image View
// epoch/src/hal/vulkan/VKImageView.h

#include <vulkan/vulkan.h>

#include "VulkanUtilities.h"

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "hal/vulkan/VKObj.h"

class VKImage;
class VKCommandPool;

class VKImageView :
	public VKObj,
	public EPFactoryMethod<VKImageView, VkPhysicalDevice, VkDevice, const EPRef<VKImage>&, VkImageAspectFlags>
{
private:
	VKImageView(
		VkPhysicalDevice vkPhysicalDevice,
		VkDevice vkLogicalDevice,
		const EPRef<VKImage>& pVKImage,
		VkImageAspectFlags vkImageAspectFlags
	);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	VKImageView(
		VkPhysicalDevice vkPhysicalDevice,
		VkDevice vkLogicalDevice
	);

	RESULT Initialize(VkImage vkImage, VkFormat vkFormat, VkImageAspectFlags vkImageAspectFlags);

	const VkImageView& GetVKImageViewHandle() const { return m_vkImageView; }

public:
	virtual ~VKImageView() override;

	static EPRef<VKImageView> InternalMake(
		VkPhysicalDevice,
		VkDevice,
		const EPRef<VKImage>&,
		VkImageAspectFlags
	);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	VkImageAspectFlags m_vkImageAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

	EPRef<VKImage> m_pVKImage = nullptr;

	VkImageView m_vkImageView = nullptr;
	
};

#endif // ! VULKAN_IMAGE_VIEW_H_