#include "VKVertexBuffer.h"

#include "VKSwapchain.h"
#include "VKCommandPool.h"

#include "VKVertex.h"
#include "VKBuffer.h"

#include "VKDescriptorSet.h"

RESULT VKVertexBuffer::Initialize() {
	RESULT r = R::OK;

// Vertex Buffer

// Staging Buffer
	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_vertices.byte_size(),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		m_vkStagingBuffer,
		m_vkStagingBufferDeviceMemory),
		"Failed to Create Vertex Buffer");
	CN(m_vkStagingBuffer);
	CN(m_vkStagingBufferDeviceMemory);

	CRM(VKBuffer::CopyDataToBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_vkStagingBufferDeviceMemory,
		m_vertices),
		"Failed to copy vertex data");

// Vertex Buffer
	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_vertices.byte_size(),
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
		m_vertices.byte_size()),
		"Copy from staging to vertex buffer failed");

	vkDestroyBuffer(m_vkLogicalDevice, m_vkStagingBuffer, nullptr);
	vkFreeMemory(m_vkLogicalDevice, m_vkStagingBufferDeviceMemory, nullptr);

// Index Buffer

// Staging Buffer
	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_indices.byte_size(),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		m_vkStagingBuffer,
		m_vkStagingBufferDeviceMemory),
		"Failed to Create Vertex Buffer");
	CN(m_vkStagingBuffer);
	CN(m_vkStagingBufferDeviceMemory);

	CRM(VKBuffer::CopyDataToBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_vkStagingBufferDeviceMemory,
		m_indices),
		"Failed to copy vertex data");

// Index Buffer
	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_indices.byte_size(),
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		m_vkIndexBuffer,
		m_vkIndexBufferDeviceMemory),
		"Failed to Create Vertex Buffer");
	CN(m_vkIndexBuffer);
	CN(m_vkIndexBufferDeviceMemory);

	CRM(VKBuffer::CopyBuffer(
		m_vkLogicalDevice,
		m_pVKCommandPool->GetVKCommandPoolHandle(),
		m_vkQueue,
		m_vkStagingBuffer,
		m_vkIndexBuffer,
		m_indices.byte_size()),
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

	// Bind the vertex and index buffers
	vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vkBuffers, vkOffsets);
	vkCmdBindIndexBuffer(vkCommandBuffer, m_vkIndexBuffer, 0, VK_INDEX_TYPE_UINT16);

Error:
	return r;
}

RESULT VKVertexBuffer::DrawIndexed(VkCommandBuffer vkCommandBuffer) {
	RESULT r = R::OK;

	CN(vkCommandBuffer);

	vkCmdDrawIndexed(vkCommandBuffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);

Error:
	return r;
}

RESULT VKVertexBuffer::Draw(VkCommandBuffer vkCommandBuffer) {
	RESULT r = R::OK;

	CN(vkCommandBuffer);

	vkCmdDraw(vkCommandBuffer, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0);

Error:
	return r;
}

RESULT VKVertexBuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	
	CN(m_vkVertexBuffer);
	CN(m_vkVertexBufferDeviceMemory);

	vkDestroyBuffer(m_vkLogicalDevice, m_vkVertexBuffer, nullptr);

	CN(m_vkVertexBufferDeviceMemory);
	vkFreeMemory(m_vkLogicalDevice, m_vkVertexBufferDeviceMemory, nullptr);

	CN(m_vkIndexBuffer);
	CN(m_vkIndexBufferDeviceMemory);

	vkDestroyBuffer(m_vkLogicalDevice, m_vkIndexBuffer, nullptr);

	CN(m_vkVertexBufferDeviceMemory);
	vkFreeMemory(m_vkLogicalDevice, m_vkIndexBufferDeviceMemory, nullptr);

Error:
	return r;
}

EPRef<VKVertexBuffer> VKVertexBuffer::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	EPRef<VKCommandPool> pVKCommandPool,
	VkQueue vkQueue
) {
	RESULT r = R::OK;
	EPRef<VKVertexBuffer> pVKVertexBuffer = nullptr;

	pVKVertexBuffer = new VKVertexBuffer(vkPhysicalDevice, vkLogicalDevice, pVKCommandPool, vkQueue);
	CNM(pVKVertexBuffer, "Failed to allocate vk vertex buffer");

	//pVKVertexBuffer->InitializeAsTriangle();
	//pVKVertexBuffer->InitializeAsQuad();
	pVKVertexBuffer->InitializeAsCube();

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
	VkQueue vkQueue
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKCommandPool(pVKCommandPool),
	m_vkQueue(vkQueue)
{
	//
}

VKVertexBuffer::~VKVertexBuffer() {
	Kill();
}