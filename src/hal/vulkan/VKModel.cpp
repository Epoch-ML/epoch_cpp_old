#include "VKModel.h"

#include "VKSwapchain.h"
#include "VKCommandPool.h"

#include "VKVertex.h"
#include "VKBuffer.h"

#include "VKDescriptorSet.h"
#include "VKDepthAttachment.h"

RESULT VKModel::Initialize() {
	RESULT r = R::OK;

	// Vertex buffer

Error:
	return r;
}

RESULT VKModel::Bind(VkCommandBuffer vkCommandBuffer) {
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

RESULT VKModel::Draw(VkCommandBuffer vkCommandBuffer) {
	RESULT r = R::OK;

	CN(vkCommandBuffer);

	vkCmdDraw(vkCommandBuffer, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0);

Error:
	return r;
}

RESULT VKModel::Kill() {
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

EPRef<VKModel> VKModel::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	EPRef<VKCommandPool> pVKCommandPool,
	const EPString<char>& strFilename
) {
	RESULT r = R::OK;
	EPRef<VKModel> pVKModel = nullptr;

	pVKModel = new VKModel(vkPhysicalDevice, vkLogicalDevice, pVKCommandPool, strFilename);
	CNM(pVKModel, "Failed to allocate vk vertex buffer");

	CRM(pVKModel->Initialize(), "Failed to initialize VK vertex buffer");

Success:
	return pVKModel;

Error:
	pVKModel = nullptr;
	return nullptr;
}

VKModel::VKModel(VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	EPRef<VKCommandPool> pVKCommandPool,
	const EPString<char> &strFilename
) :
	model(strFilename),
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKCommandPool(pVKCommandPool)
{
	//
}

VKModel::~VKModel() {
	Kill();
}