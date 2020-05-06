#include "VKVertexBuffer.h"

#include "VKVertex.h"

RESULT VKVertexBuffer::Initialize() {
	RESULT r = R::OK;

	VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties = {};

	CNM(m_vkLogicalDevice, "Cannot initialize framebuffer without valid logical device");

	m_vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	m_vkBufferCreateInfo.size = (uint64_t)(m_size);
	m_vkBufferCreateInfo.usage = m_vkBufferUsageFlags;
	m_vkBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CVKRM(vkCreateBuffer(m_vkLogicalDevice, &m_vkBufferCreateInfo, nullptr, &m_vkBuffer),
		"Failed to create vk buffer");
	CNM(m_vkBuffer, "Failed to create vk buffer");

	VkMemoryPropertyFlags vkMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

	CN(m_vkBuffer);

	// Get memory requirements and physical device memory properties 
	vkGetBufferMemoryRequirements(m_vkLogicalDevice, m_vkBuffer, &m_vkMemoryRequirements);
	vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &vkPhysicalDeviceMemoryProperties);

	// Find suitable memory type
	uint32_t memoryTypeIndex;
	bool fFound = false;
	for (memoryTypeIndex = 0; memoryTypeIndex < vkPhysicalDeviceMemoryProperties.memoryTypeCount; memoryTypeIndex++) {
		if ((m_vkMemoryRequirements.memoryTypeBits & (1 << memoryTypeIndex)) &&
			(vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & vkMemoryPropertyFlags) == vkMemoryPropertyFlags)
		{
			fFound = true;
			break;
		}
	}

	CBM(fFound, "Failed to find suitable memory type");

	// Allocate 
	m_vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_vkMemoryAllocateInfo.allocationSize = m_vkMemoryRequirements.size;
	m_vkMemoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

	CVKRM(vkAllocateMemory(m_vkLogicalDevice, &m_vkMemoryAllocateInfo, nullptr, &m_vkBufferDeviceMemory),
		"Failed to allocate buffer memory");
	CNM(m_vkBufferDeviceMemory, "Failed to allocate buffer memory");

	CVKRM(vkBindBufferMemory(m_vkLogicalDevice, m_vkBuffer, m_vkBufferDeviceMemory, 0),
		"Failed to bing memory to buffer");

Error:
	return r;
}

RESULT VKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer) {
	RESULT r = R::OK;

	VkBuffer vkBuffers[] = { m_vkBuffer };
	VkDeviceSize vkOffsets[] = { 0 };

	CN(vkCommandBuffer);

	vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vkBuffers, vkOffsets);

Error:
	return r;
}

RESULT VKVertexBuffer::CopyDataToBuffer(void* pVufferToCopy, size_t pVufferToCopy_n){
	RESULT r = R::OK;

	void* pMemoryMappedData = nullptr;

	CVKRM(vkMapMemory(m_vkLogicalDevice, m_vkBufferDeviceMemory, 0, m_vkMemoryRequirements.size, 0, &pMemoryMappedData),
		"Failed to map memory to pointer");

	memcpy(pMemoryMappedData, pVufferToCopy, pVufferToCopy_n);

	vkUnmapMemory(m_vkLogicalDevice, m_vkBufferDeviceMemory);

Error:
	return r;
}

RESULT VKVertexBuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	CN(m_vkBuffer);

	vkDestroyBuffer(m_vkLogicalDevice, m_vkBuffer, nullptr);

	CN(m_vkBufferDeviceMemory);
	vkFreeMemory(m_vkLogicalDevice, m_vkBufferDeviceMemory, nullptr);

Error:
	return r;
}

RESULT VKVertexBuffer::InitializeAsTriangle() {
	RESULT r = R::OK;

	// Set up the command buffer data
	// TODO: This is temporary just for testing
	EPVector<VKVertex<float, 2>> vertices = {
		VKVertex<float, 2>({0.0f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}),
		VKVertex<float, 2>({0.5f, 0.5f}, {0.0f, 0.0f, 0.0f, 1.0f}),
		VKVertex<float, 2>({-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f})
	};

	CRM(CopyDataToBuffer(vertices.data(), sizeof(VKVertex<float, 2>) * vertices.size()),
		"Failed to copy triangle vertex data");

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

	// TODO: obviously temp
	CR(pVKVertexBuffer->InitializeAsTriangle());

Success:
	return pVKVertexBuffer;

Error:
	pVKVertexBuffer = nullptr;
	return nullptr;
}