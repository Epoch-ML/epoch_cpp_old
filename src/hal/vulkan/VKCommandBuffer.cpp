#include "VKCommandBuffer.h"

#include "VulkanUtilities.h"

RESULT VKCommandBuffer::Initialize() {
	RESULT r = R::OK;
	EPVector<uint32_t> queueFamilies;

	CN(m_vkPhysicalDevice);
	CN(m_vkLogicalDevice);
	CN(m_vkSurface);

	// TODO: This is too loosey goosey 
	queueFamilies = FindQueueFamilies(m_vkPhysicalDevice, m_vkSurface);

	m_vkCommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_vkCommandPoolCreateInfo.queueFamilyIndex = queueFamilies[0];
	m_vkCommandPoolCreateInfo.flags = 0; // Optional

	CVKRM(vkCreateCommandPool(m_vkLogicalDevice, &m_vkCommandPoolCreateInfo, nullptr, &m_vkCommandPool),
		"Failed to create vk command pool");
	CNM(m_vkCommandPool, "Failed to create command pool");

Error:
	return r;
}

RESULT VKCommandBuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	CN(m_vkCommandPool);

	vkDestroyCommandPool(m_vkLogicalDevice, m_vkCommandPool, nullptr);

Error:
	return r;
}

EPRef<VKCommandBuffer> VKCommandBuffer::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice, 
	VkSurfaceKHR vkSurface,
	const EPRef<VKPipeline>& pVKPipeline, 
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKCommandBuffer> pVKCommandBuffer = nullptr;

	pVKCommandBuffer = new VKCommandBuffer(vkPhysicalDevice, vkLogicalDevice, vkSurface, pVKPipeline, pVKSwapchain);
	CNM(pVKCommandBuffer, "Failed to allocate vk framebuffer");

	CRM(pVKCommandBuffer->Initialize(), "Failed to initialize VK framebufer");

Success:
	return pVKCommandBuffer;

Error:
	pVKCommandBuffer = nullptr;
	return nullptr;
}