#include "VKDepthAttachment.h"

#include "VulkanUtilities.h"

#include "VKSwapchain.h"
#include "VKImage.h"
#include "VKImageView.h"
#include "VKCommandPool.h"

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

	//// Transition the image layout
	//CRM(m_pVKImage->TranisitionImageLayout(m_pVKCommandPool,
	//	VK_IMAGE_LAYOUT_UNDEFINED,
	//	VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	//	VK_IMAGE_ASPECT_DEPTH_BIT,
	//	m_pVKCommandPool->GetVKQueueHandle()),
	//	"Failed to transition image layout");

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
	EPRef<VKSwapchain> pVKSwapchain,
	const EPRef<VKCommandPool>& pVKCommandPool
) {
	RESULT r = R::OK;
	EPRef<VKDepthAttachment> pVKDepthAttachment = nullptr;

	pVKDepthAttachment = new VKDepthAttachment(
		vkPhysicalDevice, 
		vkLogicalDevice,
		pVKSwapchain,
		pVKCommandPool
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
	EPRef<VKSwapchain> pVKSwapchain,
	const EPRef<VKCommandPool>& pVKCommandPool
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKSwapchain(pVKSwapchain),
	m_pVKCommandPool(pVKCommandPool)
{
	//
}

VKDepthAttachment::~VKDepthAttachment() {
	Kill();
}

const VkAttachmentDescription VKDepthAttachment::GetVKAttachmentDescription() const {
	VkAttachmentDescription vkDepthAttachmentDescription = {};

	vkDepthAttachmentDescription.format = m_vkDepthFormat;
	vkDepthAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	vkDepthAttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	vkDepthAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vkDepthAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	vkDepthAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	vkDepthAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vkDepthAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	return vkDepthAttachmentDescription;
}

const VkFormat VKDepthAttachment::GetVKFormat() const { 
	return m_vkDepthFormat; 
}

const VkImage VKDepthAttachment::GetVKImageHandle() const { 
	return m_pVKImage->GetVKImageHandle(); 
}

const VkImageView VKDepthAttachment::GetVKImageViewHandle() const { 
	return m_pVKImageView->GetVKImageViewHandle(); 
}