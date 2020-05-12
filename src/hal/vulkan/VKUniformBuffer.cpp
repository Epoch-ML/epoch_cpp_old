#include "VKUniformBuffer.h"

#include "VKCommandPool.h"

#include "VKVertex.h"
#include "VKBuffer.h"

RESULT VKUniformBuffer::Initialize() {
	RESULT r = R::OK;

	// Set up the buffers and device memory for the uniform buffer

	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	
	m_vkUniformBuffers = EPVector<VkBuffer>(m_pVKSwapchain->GetSwapchainImageCount(), true);
	m_vkUniformBufferDeviceMemories = EPVector<VkDeviceMemory>(m_pVKSwapchain->GetSwapchainImageCount(), true);
	
	for (size_t i = 0; i < m_pVKSwapchain->GetSwapchainImageCount(); i++) {

		CVKRM(VKBuffer::CreateBuffer(
			m_vkPhysicalDevice,
			m_vkLogicalDevice,
			bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_vkUniformBuffers[i],
			m_vkUniformBufferDeviceMemories[i]),
			"Failed to Create Uniform Buffer");
		
		CN(m_vkUniformBuffers[i]);
		CN(m_vkUniformBufferDeviceMemories[i]);
	}

Error:
	return r;
}

RESULT VKUniformBuffer::Update(uint32_t currentImage) {
	RESULT r = R::OK;



Error:
	return r;
}

RESULT VKUniformBuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);

	for (size_t i = 0; i < m_pVKSwapchain->GetSwapchainImageCount(); i++) {
		//createBuffer(bufferSize, , , uniformBuffers[i], uniformBuffersMemory[i]);

		CN(m_vkUniformBuffers[i]);
		vkDestroyBuffer(m_vkLogicalDevice, m_vkUniformBuffers[i], nullptr);

		CN(m_vkUniformBufferDeviceMemories[i]);
		vkFreeMemory(m_vkLogicalDevice, m_vkUniformBufferDeviceMemories[i], nullptr);

		m_vkUniformBuffers[i] = nullptr;
		m_vkUniformBufferDeviceMemories[i] = nullptr;
	}

Error:
	return r;
}

EPRef<VKUniformBuffer> VKUniformBuffer::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKUniformBuffer> pVKUniformBuffer = nullptr;

	pVKUniformBuffer = new VKUniformBuffer(vkPhysicalDevice, vkLogicalDevice, pVKSwapchain);
	CNM(pVKUniformBuffer, "Failed to allocate vk vertex buffer");

	CRM(pVKUniformBuffer->Initialize(), "Failed to initialize VK vertex buffer");

Success:
	return pVKUniformBuffer;

Error:
	pVKUniformBuffer = nullptr;
	return nullptr;
}

VKUniformBuffer::VKUniformBuffer(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKSwapchain> &pVKSwapchain
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKSwapchain(pVKSwapchain)
{
	//
}

VKUniformBuffer::~VKUniformBuffer() {
	Kill();
}