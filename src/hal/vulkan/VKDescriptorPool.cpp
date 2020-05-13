#include "VKDescriptorPool.h"

#include "VulkanUtilities.h"

RESULT VKDescriptorPool::Initialize() {
	RESULT r = R::OK;

	CNM(m_vkLogicalDevice, "Cannot initialize command pool without valid logical device");
	CNM(m_pVKSwapchain, "Cannot initialize command pool without valid swapchain");

	m_vkDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	m_vkDescriptorPoolSize.descriptorCount = static_cast<uint32_t>(m_pVKSwapchain->GetSwapchainImageCount());

	m_vkDescriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	m_vkDescriptorPoolCreateInfo.poolSizeCount = 1;
	m_vkDescriptorPoolCreateInfo.pPoolSizes = &m_vkDescriptorPoolSize;
	m_vkDescriptorPoolCreateInfo.maxSets = static_cast<uint32_t>(m_pVKSwapchain->GetSwapchainImageCount());

	CVKRM(vkCreateDescriptorPool(m_vkLogicalDevice, &m_vkDescriptorPoolCreateInfo, nullptr, &m_vkDescriptorPool),
		"Failed to create descriptor pool");

Error:
	return r;
}

RESULT VKDescriptorPool::Kill() {
	RESULT r = R::OK;

	CN(m_vkDescriptorPool);

	vkDestroyDescriptorPool(m_vkLogicalDevice, m_vkDescriptorPool, nullptr);

Error:
	return r;
}

EPRef<VKDescriptorPool> VKDescriptorPool::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKDescriptorPool> pVKDescriptorPool = nullptr;

	pVKDescriptorPool = new VKDescriptorPool(vkPhysicalDevice, vkLogicalDevice, pVKSwapchain);
	CNM(pVKDescriptorPool, "Failed to allocate vk command pool");

	CRM(pVKDescriptorPool->Initialize(), "Failed to initialize VK command pool");

Success:
	return pVKDescriptorPool;

Error:
	pVKDescriptorPool = nullptr;
	return nullptr;
}