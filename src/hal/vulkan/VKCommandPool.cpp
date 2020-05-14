#include "VKCommandPool.h"

#include "VulkanUtilities.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"
#include "VKCommandBuffers.h"

#include "VKDescriptorSet.h"

RESULT VKCommandPool::Initialize() {
	RESULT r = R::OK;
	VKQueueFamilies vkQueueFamilies;

	CNM(m_vkLogicalDevice, "Cannot initialize command pool without valid logical device");
	CNM(m_pVKPipeline, "Cannot initialize command pool without valid pipeline");
	CNM(m_pVKSwapchain, "Cannot initialize command pool without valid swapchain");
	CN(m_vkSurface);

	// TODO: This is too loosey goosey 

	vkQueueFamilies = FindQueueFamilies(m_vkPhysicalDevice, m_vkSurface);

	m_vkCommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_vkCommandPoolCreateInfo.queueFamilyIndex = vkQueueFamilies.GetGraphicsQueueIndex();
	m_vkCommandPoolCreateInfo.flags = 0; // Optional

	CVKRM(vkCreateCommandPool(m_vkLogicalDevice, &m_vkCommandPoolCreateInfo, nullptr, &m_vkCommandPool),
		"Failed to create vk command pool");
	CNM(m_vkCommandPool, "Failed to create command pool");

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
	VkQueue vkQueue,
	const EPRef<VKPipeline>& pVKPipeline, 
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKCommandPool> pVKCommandPool = nullptr;

	pVKCommandPool = new VKCommandPool(vkPhysicalDevice, vkLogicalDevice, vkSurface, vkQueue, pVKPipeline, pVKSwapchain);
	CNM(pVKCommandPool, "Failed to allocate vk command pool");

	CRM(pVKCommandPool->Initialize(), "Failed to initialize VK command pool");

Success:
	return pVKCommandPool;

Error:
	pVKCommandPool = nullptr;
	return nullptr;
}

EPRef<VKCommandBuffers> VKCommandPool::MakeCommandBuffers(
	const EPRef<VKVertexBuffer>& pVKVertextBuffer, 
	const EPRef<VKDescriptorSet>& pVKDescriptorSet
) {
	RESULT r = R::OK;

	EPRef<VKCommandBuffers> pVKCommandBuffers = VKCommandBuffers::make(
		*(new EPRef<VKCommandPool>(this)), 
		pVKVertextBuffer,
		pVKDescriptorSet);

	CNM(pVKCommandBuffers, "Failed to make vk command buffers");

Success:
	return pVKCommandBuffers;

Error:
	return nullptr;
}

VKCommandPool::VKCommandPool(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	VkSurfaceKHR vkSurface,
	VkQueue vkQueue,
	const EPRef<VKPipeline>& pVKPipeline,
	const EPRef<VKSwapchain>& pVKSwapchain
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_vkSurface(vkSurface),
	m_vkQueue(vkQueue),
	m_pVKPipeline(pVKPipeline),
	m_pVKSwapchain(pVKSwapchain)
{
	//
}

VKCommandPool::~VKCommandPool() {
	Kill();
}