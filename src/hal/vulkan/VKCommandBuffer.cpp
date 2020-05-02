#include "VKCommandBuffer.h"

#include "VulkanUtilities.h"

RESULT VKCommandBuffer::Initialize() {
	RESULT r = R::OK;
	
	//

Error:
	return r;
}

RESULT VKCommandBuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	CN(m_vkFramebuffer);

	//

Error:
	return r;
}

EPRef<VKCommandBuffer> VKCommandBuffer::InternalMake(
	VkDevice vkLogicalDevice, 
	const EPRef<VKPipeline>& pVKPipeline, 
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKCommandBuffer> pVKCommandBuffer = nullptr;

	pVKCommandBuffer = new VKCommandBuffer(vkLogicalDevice, pVKPipeline, pVKSwapchain);
	CNM(pVKCommandBuffer, "Failed to allocate vk framebuffer");

	CRM(pVKCommandBuffer->Initialize(), "Failed to initialize VK framebufer");

Success:
	return pVKCommandBuffer;

Error:
	pVKCommandBuffer = nullptr;
	return nullptr;
}