#include "VKCommandPool.h"

#include "VulkanUtilities.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"
#include "VKCommandBuffers.h"

#include "VKDescriptorSet.h"
#include "VKDepthAttachment.h"

RESULT VKCommandPool::Initialize() {
	RESULT r = R::OK;
	VKQueueFamilies vkQueueFamilies;

	CNM(m_vkLogicalDevice, "Cannot initialize command pool without valid logical device");
	//CNM(m_pVKPipeline, "Cannot initialize command pool without valid pipeline");
	//CNM(m_pVKSwapchain, "Cannot initialize command pool without valid swapchain");
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
	VkQueue vkQueue
) {
	RESULT r = R::OK;
	EPRef<VKCommandPool> pVKCommandPool = nullptr;

	pVKCommandPool = new VKCommandPool(vkPhysicalDevice, vkLogicalDevice, vkSurface, vkQueue);
	CNM(pVKCommandPool, "Failed to allocate vk command pool");

	CRM(pVKCommandPool->Initialize(), "Failed to initialize VK command pool");

Success:
	return pVKCommandPool;

Error:
	pVKCommandPool = nullptr;
	return nullptr;
}

EPRef<VKCommandBuffers> VKCommandPool::MakeVertexDescriptorCommandBuffers(
	const EPRef<VKPipeline>& pVKPipeline,
	const EPRef<VKSwapchain>& pVKSwapchain,
	const EPRef<VKVertexBuffer>& pVKVertextBuffer, 
	const EPRef<VKDescriptorSet>& pVKDescriptorSet
) {
	RESULT r = R::OK;

	EPRef<VKCommandBuffers> pVKCommandBuffers = VKCommandBuffers::make(
		*(new EPRef<VKCommandPool>(this)), 
		pVKVertextBuffer,
		pVKDescriptorSet,
		pVKPipeline,
		pVKSwapchain);

	CNM(pVKCommandBuffers, "Failed to make vk command buffers");

Success:
	return pVKCommandBuffers;

Error:
	return nullptr;
}

EPRef<VKCommandBuffers> VKCommandPool::MakeVertexDescriptorCommandBuffers(
	const EPRef<VKPipeline>& pVKPipeline,
	const EPRef<VKSwapchain>& pVKSwapchain,
	const EPRef<VKModel>& pVKModel,
	const EPRef<VKDescriptorSet>& pVKDescriptorSet
) {
	RESULT r = R::OK;

	EPRef<VKCommandBuffers> pVKCommandBuffers = VKCommandBuffers::MakeFromModel(
		*(new EPRef<VKCommandPool>(this)),
		pVKModel,
		pVKDescriptorSet,
		pVKPipeline,
		pVKSwapchain);

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
	VkQueue vkQueue
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_vkSurface(vkSurface),
	m_vkQueue(vkQueue)
{
	//
}

VKCommandPool::~VKCommandPool() {
	Kill();
}