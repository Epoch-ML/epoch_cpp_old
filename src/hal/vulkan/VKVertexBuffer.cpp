#include "VKVertexBuffer.h"

#include "VKCommandPool.h"

#include "VKVertex.h"
#include "VKBuffer.h"

RESULT VKVertexBuffer::Initialize() {
	RESULT r = R::OK;

// Staging Buffer
	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		m_vkStagingBuffer,
		m_vkStagingBufferDeviceMemory),
		"Failed to Create Vertex Buffer");
	CN(m_vkStagingBuffer);
	CN(m_vkStagingBufferDeviceMemory);

	// TODO: obviously temp
	CR(InitializeAsTriangle());

// Vertex Buffer
	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_size,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_vkVertexBuffer,
		m_vkVertexBufferDeviceMemory),
		"Failed to Create Vertex Buffer");
	CN(m_vkVertexBuffer);
	CN(m_vkVertexBufferDeviceMemory);

	CRM(VKBuffer::CopyBuffer(
		m_vkLogicalDevice,
		m_pVKCommandPool->GetVKCommandPoolHandle(),
		m_vkQueue,
		m_vkStagingBuffer,
		m_vkVertexBuffer,
		m_size),
		"Copy from staging to vertex buffer failed");

	vkDestroyBuffer(m_vkLogicalDevice, m_vkStagingBuffer, nullptr);
	vkFreeMemory(m_vkLogicalDevice, m_vkStagingBufferDeviceMemory, nullptr);

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

	CRM(VKBuffer::CopyDataToBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_vkStagingBufferDeviceMemory,
		vertices.data(), 
		sizeof(VKVertex<float, 2>) * vertices.size()),
		"Failed to copy triangle vertex data");

Error:
	return r;
}

EPRef<VKVertexBuffer> VKVertexBuffer::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	EPRef<VKCommandPool> pVKCommandPool,
	VkQueue vkQueue,
	size_t size
) {
	RESULT r = R::OK;
	EPRef<VKVertexBuffer> pVKVertexBuffer = nullptr;

	pVKVertexBuffer = new VKVertexBuffer(vkPhysicalDevice, vkLogicalDevice, pVKCommandPool, vkQueue, size);
	CNM(pVKVertexBuffer, "Failed to allocate vk vertex buffer");

	CRM(pVKVertexBuffer->Initialize(), "Failed to initialize VK vertex buffer");

Success:
	return pVKVertexBuffer;

Error:
	pVKVertexBuffer = nullptr;
	return nullptr;
}

VKVertexBuffer::VKVertexBuffer(VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	EPRef<VKCommandPool> pVKCommandPool,
	VkQueue vkQueue,
	size_t size
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKCommandPool(pVKCommandPool),
	m_vkQueue(vkQueue),
	m_size(size)
{
	//
}

VKVertexBuffer::~VKVertexBuffer() {
	Kill();
}