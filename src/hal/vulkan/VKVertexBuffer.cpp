#include "VKVertexBuffer.h"

#include "VulkanUtilities.h"

RESULT VKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer) {
	RESULT r = R::OK;

	VkBuffer vkBuffers[] = { m_vkBuffer };
	VkDeviceSize vkOffsets[] = { 0 };

	CN(vkCommandBuffer);

	vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vkBuffers, vkOffsets);

Error:
	return r;
}

EPRef<VKVertexBuffer> VKVertexBuffer::InternalMake(
	VkPhysicalDevice vkPhysicalDevice, 
	VkDevice vkLogicalDevice, 
	size_t size) 
{
	RESULT r = R::OK;
	EPRef<VKVertexBuffer> pVKVertexBuffer = nullptr;

	pVKVertexBuffer = new VKVertexBuffer(vkPhysicalDevice, vkLogicalDevice, size);
	CNM(pVKVertexBuffer, "Failed to allocate vk vertex buffer");

	CRM(pVKVertexBuffer->Initialize(), "Failed to initialize VK vertex buffer");

Success:
	return pVKVertexBuffer;

Error:
	pVKVertexBuffer = nullptr;
	return nullptr;
}