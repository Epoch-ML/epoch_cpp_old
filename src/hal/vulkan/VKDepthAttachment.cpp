#include "VKDepthAttachment.h"

#include "VulkanUtilities.h"

#include "VKSwapchain.h"
#include "VKImage.h"
#include "VKImageView.h"


VkFormat VKDepthAttachment::FindDepthFormat() {
	return FindSupportedFormat(
		m_vkPhysicalDevice,
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

RESULT VKDepthAttachment::Initialize() {
	RESULT r = R::OK;

	m_vkDepthFormat = FindDepthFormat();
	CBM(m_vkDepthFormat != VK_FORMAT_UNDEFINED, "Failed to find valid format");
	//CBM(VKFormatHasStencilComponent(m_vkDepthFormat), "Depth format doesn't have a stencil component");

	m_pVKImage = VKImage::make(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_pVKSwapchain->GetExtentsWidth(), m_pVKSwapchain->GetExtentsHeight(),
		m_vkDepthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);
	CNM(m_pVKImage, "Failed to create depth attachment image");

	m_pVKImageView = VKImageView::make(
		m_vkPhysicalDevice, 
		m_vkLogicalDevice, 
		m_pVKImage,
		VK_IMAGE_ASPECT_DEPTH_BIT);
	CNM(m_pVKImageView, "Failed to create depth attachment image view");

Error:
	return r;
}

RESULT VKDepthAttachment::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	
	//

Error:
	return r;
}

EPRef<VKDepthAttachment> VKDepthAttachment::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	EPRef<VKSwapchain> pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKDepthAttachment> pVKDepthAttachment = nullptr;

	pVKDepthAttachment = new VKDepthAttachment(
		vkPhysicalDevice, 
		vkLogicalDevice,
		pVKSwapchain
	);
	CNM(pVKDepthAttachment, "Failed to allocate vk Depth Attachment");

	CRM(pVKDepthAttachment->Initialize(), "Failed to initialize VK Depth Attachment");

Success:
	return pVKDepthAttachment;

Error:
	pVKDepthAttachment = nullptr;
	return nullptr;
}

VKDepthAttachment::VKDepthAttachment(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	EPRef<VKSwapchain> pVKSwapchain
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKSwapchain(pVKSwapchain)
{
	//
}

VKDepthAttachment::~VKDepthAttachment() {
	Kill();
}