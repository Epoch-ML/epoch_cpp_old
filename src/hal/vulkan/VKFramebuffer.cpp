#include "VKFramebuffer.h"

#include "VulkanUtilities.h"

RESULT VKFramebuffer::Initialize() {
	RESULT r = R::OK;
	
	CNM(m_vkLogicalDevice, "Cannot initialize framebuffer without valid logical device");
	CNM(m_pVKPipeline, "Cannot initialize framebuffer without valid pipeline");
	CNM(m_pVKSwapchain, "Cannot initialize framebuffer without valid swapchain");

	m_vkFramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	m_vkFramebufferCreateInfo.renderPass = m_pVKPipeline->GetVKRenderPass();
	m_vkFramebufferCreateInfo.attachmentCount = 1;
	m_vkFramebufferCreateInfo.pAttachments = m_pVKSwapchain->GetSwapchainImageViews();
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
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKFramebuffer> pVKFramebuffer = nullptr;

	pVKFramebuffer = new VKFramebuffer(vkLogicalDevice, pVKPipeline, pVKSwapchain);
	CNM(pVKFramebuffer, "Failed to allocate vk framebuffer");

	CRM(pVKFramebuffer->Initialize(), "Failed to initialize VK framebufer");

Success:
	return pVKFramebuffer;

Error:
	pVKFramebuffer = nullptr;
	return nullptr;
}