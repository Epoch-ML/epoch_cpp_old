#include "VKVertexBuffer.h"

#include "VKVertex.h"
#include "VKBuffer.h"

RESULT VKVertexBuffer::Initialize() {
	RESULT r = R::OK;

// Vertex Buffer
	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_size,
		m_vkBufferUsageFlags,
		m_vkMemoryPropertyFlags,
		m_vkVertexBuffer,
		m_vkVertexBufferDeviceMemory),
		"Failed to Create Vertex Buffer");
	CN(m_vkVertexBuffer);
	CN(m_vkVertexBufferDeviceMemory);

Error:
	return r;
}

RESULT VKVertexBuffer::Bind(VkCommandBuffer vkCommandBuffer) {
	RESULT r = R::OK;

	VkBuffer vkBuffers[] = { m_vkVertexBuffer };
	VkDeviceSize vkOffsets[] = { 0 };

	CN(vkCommandBuffer);

	vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vkBuffers, vkOffsets);

Error:
	return r;
}

RESULT VKVertexBuffer::CopyDataToBuffer(void* pVufferToCopy, size_t pVufferToCopy_n){
	RESULT r = R::OK;

	void* pMemoryMappedData = nullptr;

	CVKRM(vkMapMemory(m_vkLogicalDevice, m_vkVertexBufferDeviceMemory, 0, pVufferToCopy_n, 0, &pMemoryMappedData),
		"Failed to map memory to pointer");

	memcpy(pMemoryMappedData, pVufferToCopy, pVufferToCopy_n);

	vkUnmapMemory(m_vkLogicalDevice, m_vkVertexBufferDeviceMemory);

Error:
	return r;
}

RESULT VKVertexBuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	CN(m_vkVertexBuffer);

	vkDestroyBuffer(m_vkLogicalDevice, m_vkVertexBuffer, nullptr);

	CN(m_vkVertexBufferDeviceMemory);
	vkFreeMemory(m_vkLogicalDevice, m_vkVertexBufferDeviceMemory, nullptr);

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