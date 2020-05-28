#include "VKModel.h"

#include "VKSwapchain.h"
#include "VKCommandPool.h"

#include "VKVertex.h"
#include "VKVertexBuffer.h"
#include "VKBuffer.h"

#include "VKDescriptorSet.h"
#include "VKDepthAttachment.h"

#include "core/model/TOLModel.h"

RESULT VKModel::Initialize() {
	RESULT r = R::OK;

	// Vertex buffer
	EPRef<TOLModel> pTOLModel = nullptr;

	pTOLModel = TOLModel::make(m_strModelFilename, m_strTextureFilename);
	CNM(pTOLModel, "Failed to load TOL model from disk");

	// TODO: this is not using the factory - revisit
	m_pVKVertexBuffer = new VKVertexBuffer(
		m_vkPhysicalDevice, 
		m_vkLogicalDevice, 
		m_pVKCommandPool, 
		m_pVKCommandPool->GetVKQueueHandle());
	CNM(m_pVKVertexBuffer, "Failed to allocate vk vertex buffer");

	CRM(m_pVKVertexBuffer->Set(pTOLModel->GetVertices(), pTOLModel->GetIndices()),
		"Failed to set model indices and vertices");

	CRM(m_pVKVertexBuffer->Initialize(), "Failed to initialize VK vertex buffer");

Error:
	return r;
}

RESULT VKModel::Bind(VkCommandBuffer vkCommandBuffer) {
	RESULT r = R::OK;

	CN(vkCommandBuffer);

	CRM(m_pVKVertexBuffer->Bind(vkCommandBuffer), "Failed to bind vk vertex buffer");

Error:
	return r;
}

RESULT VKModel::Draw(VkCommandBuffer vkCommandBuffer) {
	RESULT r = R::OK;

	CN(vkCommandBuffer);

	CRM(m_pVKVertexBuffer->DrawIndexed(vkCommandBuffer), "Failed to draw vk vertex buffer");
	//CRM(m_pVKVertexBuffer->Draw(vkCommandBuffer), "Failed to draw vk vertex buffer");

Error:
	return r;
}

RESULT VKModel::Kill() {
	RESULT r = R::OK;

	m_pVKVertexBuffer = nullptr;

	// TODO: move textures into this object
	//m_pVKTexture = nullptr;

Error:
	return r;
}

EPRef<VKModel> VKModel::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	EPRef<VKCommandPool> pVKCommandPool,
	const EPString<char>& strModelFilename,
	const EPString<char>& strTextureFilename
) {
	RESULT r = R::OK;
	EPRef<VKModel> pVKModel = nullptr;

	pVKModel = new VKModel(vkPhysicalDevice, vkLogicalDevice, pVKCommandPool, strModelFilename, strTextureFilename);
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
	const EPString<char> &strModelFilename, const EPString<char>& strTextureFilename
) :
	model(strModelFilename, strTextureFilename),
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKCommandPool(pVKCommandPool)
{
	//
}

VKModel::~VKModel() {
	Kill();
}