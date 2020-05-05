#include "VKCommandPool.h"

#include "VulkanUtilities.h"

RESULT VKCommandPool::Initialize() {
	RESULT r = R::OK;
	EPVector<uint32_t> queueFamilies;

	CNM(m_vkLogicalDevice, "Cannot initialize command pool without valid logical device");
	CNM(m_pVKPipeline, "Cannot initialize command pool without valid pipeline");
	CNM(m_pVKSwapchain, "Cannot initialize command pool without valid swapchain");
	CN(m_vkSurface);

	// TODO: This is too loosey goosey 
	queueFamilies = FindQueueFamilies(m_vkPhysicalDevice, m_vkSurface);

	m_vkCommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_vkCommandPoolCreateInfo.queueFamilyIndex = queueFamilies[0];
	m_vkCommandPoolCreateInfo.flags = 0; // Optional

	CVKRM(vkCreateCommandPool(m_vkLogicalDevice, &m_vkCommandPoolCreateInfo, nullptr, &m_vkCommandPool),
		"Failed to create vk command pool");
	CNM(m_vkCommandPool, "Failed to create command pool");

	CRM(InitializeCommandBuffers(), "Failed to set up command buffers");

Error:
	return r;
}

RESULT VKCommandPool::Kill() {
	RESULT r = R::OK;

	CN(m_vkCommandPool);

	vkDestroyCommandPool(m_vkLogicalDevice, m_vkCommandPool, nullptr);

Error:
	return r;
}

EPRef<VKCommandPool> VKCommandPool::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice, 
	VkSurfaceKHR vkSurface,
	const EPRef<VKPipeline>& pVKPipeline, 
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKCommandPool> pVKCommandPool = nullptr;

	pVKCommandPool = new VKCommandPool(vkPhysicalDevice, vkLogicalDevice, vkSurface, pVKPipeline, pVKSwapchain);
	CNM(pVKCommandPool, "Failed to allocate vk command pool");

	CRM(pVKCommandPool->Initialize(), "Failed to initialize VK command pool");

Success:
	return pVKCommandPool;

Error:
	pVKCommandPool = nullptr;
	return nullptr;
}

RESULT VKCommandPool::InitializeCommandBuffers() {
	RESULT r = R::OK;

	// TODO: This is nasty (the const ref doesn't create a new reference so when the underlying object 
	// is killed it will null out the reference and kill our object
	m_pVKCommandBuffers = VKCommandBuffers::make(*(new EPRef<VKCommandPool>(this)));
	CNM(m_pVKCommandBuffers, "Failed to make vk command buffers");

Error:
	return r;
}