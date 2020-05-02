#include "VKCommandPool.h"

#include "VulkanUtilities.h"

RESULT VKFramebuffer::Initialize() {
	RESULT r = R::OK;
	
	CNM(m_vkLogicalDevice, "Cannot initialize command pool without valid logical device");
	CNM(m_pVKPipeline, "Cannot initialize command pool without valid pipeline");
	CNM(m_pVKSwapchain, "Cannot initialize command pool without valid swapchain");

	//

Error:
	return r;
}

RESULT VKFramebuffer::Kill() {
	RESULT r = R::OK;

	//

Error:
	return r;
}

EPRef<VKCommandPool> VKCommandPool::InternalMake(
	VkDevice vkLogicalDevice, 
	const EPRef<VKPipeline>& pVKPipeline, 
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKCommandPool> pVKCommandPool = nullptr;

	pVKCommandPool = new VKCommandPool(vkLogicalDevice, pVKPipeline, pVKSwapchain);
	CNM(pVKCommandPool, "Failed to allocate vk command pool");

	CRM(pVKCommandPool->Initialize(), "Failed to initialize VK command pool");

Success:
	return pVKCommandPool;

Error:
	pVKCommandPool = nullptr;
	return nullptr;
}