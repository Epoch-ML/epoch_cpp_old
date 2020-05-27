#include "VKImageView.h"

#include "VKImage.h"

RESULT VKImageView::Initialize(VkImage vkImage, VkFormat vkFormat, VkImageAspectFlags vkImageAspectFlags) {
	RESULT r = R::OK;

	VkImageViewCreateInfo vkImageViewCreateInfo = {};

	m_vkImageAspectFlags = vkImageAspectFlags;

	CN(m_vkLogicalDevice);
	CN(vkImage);

	vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	vkImageViewCreateInfo.image = vkImage;
	vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	vkImageViewCreateInfo.format = vkFormat;
	vkImageViewCreateInfo.subresourceRange.aspectMask = m_vkImageAspectFlags;
	vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	vkImageViewCreateInfo.subresourceRange.levelCount = 1;
	vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	vkImageViewCreateInfo.subresourceRange.layerCount = 1;
	vkImageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY };

	CVKRM(vkCreateImageView(m_vkLogicalDevice, &vkImageViewCreateInfo, nullptr, &m_vkImageView),
		"Failed to create image view");
	CNM(m_vkImageView, "Failed to allocated image view");

Error:
	return r;
}

RESULT VKImageView::Initialize() {
	return Initialize(
		m_pVKImage->GetVKImageHandle(), 
		VK_FORMAT_R8G8B8A8_SRGB, 
		VK_IMAGE_ASPECT_COLOR_BIT
	);
}

RESULT VKImageView::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	CN(m_vkImageView);

	vkDestroyImageView(m_vkLogicalDevice, m_vkImageView, nullptr);

Error:
	return r;
}

EPRef<VKImageView> VKImageView::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKImage>& pVKImage,
	VkImageAspectFlags vkImageAspectFlags
) {
	RESULT r = R::OK;
	EPRef<VKImageView> pVKImageView = nullptr;

	pVKImageView = new VKImageView(
		vkPhysicalDevice,
		vkLogicalDevice,
		pVKImage,
		vkImageAspectFlags
	);
	CNM(pVKImageView, "Failed to allocate vk image view");

	CRM(pVKImageView->Initialize(), "Failed to initialize VK image view");

Success:
	return pVKImageView;

Error:
	pVKImageView = nullptr;
	return nullptr;
}

VKImageView::VKImageView(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKImage>& pVKImage,
	VkImageAspectFlags vkImageAspectFlags
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKImage(pVKImage),
	m_vkImageAspectFlags(vkImageAspectFlags)
{
	//
}

VKImageView::VKImageView(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKImage(nullptr),
	m_vkImageAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT)
{
	//
}

VKImageView::~VKImageView() {
	Kill();
}