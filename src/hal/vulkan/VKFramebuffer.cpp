#include "VKFramebuffer.h"

#include "VulkanUtilities.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"

#include "VKDescriptorSet.h"

RESULT VKFramebuffer::Initialize() {
	RESULT r = R::OK;
	
	CNM(m_vkLogicalDevice, "Cannot initialize framebuffer without valid logical device");
	CNM(m_pVKPipeline, "Cannot initialize framebuffer without valid pipeline");
	CNM(m_pVKSwapchain, "Cannot initialize framebuffer without valid swapchain");

	m_vkFramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	m_vkFramebufferCreateInfo.renderPass = m_pVKPipeline->GetVKRenderPassHandle();
	m_vkFramebufferCreateInfo.attachmentCount = 1;
	m_vkFramebufferCreateInfo.pAttachments = &(m_pVKSwapchain->GetSwapchainImageView(m_frameBufferIndex));
	m_vkFramebufferCreateInfo.width = m_pVKSwapchain->GetExtentsWidth();
	m_vkFramebufferCreateInfo.height = m_pVKSwapchain->GetExtentsHeight();
	m_vkFramebufferCreateInfo.layers = 1;

	CVKRM(vkCreateFramebuffer(m_vkLogicalDevice, &m_vkFramebufferCreateInfo, nullptr, &m_vkFramebuffer),
		"Failed to create framebuffer");
	CNM(m_vkFramebuffer, "Failed to create framebuffer");

Error:
	return r;
}

RESULT VKFramebuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	CN(m_vkFramebuffer);

	vkDestroyFramebuffer(m_vkLogicalDevice, m_vkFramebuffer, nullptr);

Error:
	return r;
}

EPRef<VKFramebuffer> VKFramebuffer::InternalMake(
	VkDevice vkLogicalDevice, 
	const EPRef<VKPipeline>& pVKPipeline, 
	const EPRef<VKSwapchain>& pVKSwapchain,
	uint32_t frameBufferIndex
) {
	RESULT r = R::OK;
	EPRef<VKFramebuffer> pVKFramebuffer = nullptr;

	pVKFramebuffer = new VKFramebuffer(vkLogicalDevice, pVKPipeline, pVKSwapchain, frameBufferIndex);
	CNM(pVKFramebuffer, "Failed to allocate vk framebuffer");

	CRM(pVKFramebuffer->Initialize(), "Failed to initialize VK framebufer");

Success:
	return pVKFramebuffer;

Error:
	pVKFramebuffer = nullptr;
	return nullptr;
}

VKFramebuffer::VKFramebuffer(
	VkDevice vkLogicalDevice,
	const EPRef<VKPipeline>& pVKPipeline,
	const EPRef<VKSwapchain>& pVKSwapchain,
	uint32_t frameBufferIndex
) :
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKPipeline(pVKPipeline),
	m_pVKSwapchain(pVKSwapchain),
	m_frameBufferIndex(frameBufferIndex)
{
	//
}


VKFramebuffer::~VKFramebuffer() {
	Kill();
}