#include "VKCommandBuffers.h"

#include "VulkanUtilities.h"

#include "VKCommandPool.h"

VKCommandBuffers::VKCommandBuffers(const EPRef<VKCommandPool>& pVKCommandPool) :
	m_pVKCommandPool(pVKCommandPool)
{
	//
}

VKCommandBuffers::~VKCommandBuffers() {
	Kill();
}

RESULT VKCommandBuffers::Initialize() {
	RESULT r = R::OK;

	CN(m_pVKCommandPool);
	
	m_vkCommandBuffers = EPVector<VkCommandBuffer>(2, true);

	m_vkCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_vkCommandBufferAllocateInfo.commandPool = m_pVKCommandPool->GetVKCommandPoolHandle();
	m_vkCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_vkCommandBufferAllocateInfo.commandBufferCount = (uint32_t)m_vkCommandBuffers.size();

	CVKRM(vkAllocateCommandBuffers(m_pVKCommandPool->GetLogicalDeviceHandle(), &m_vkCommandBufferAllocateInfo, m_vkCommandBuffers.data()),
		"Failed to allocate command buffers");

Error:
	return r;
}

RESULT VKCommandBuffers::Kill() {
	RESULT r = R::OK;

	CN(m_pVKCommandPool);

Error:
	return r;
}

EPRef<VKCommandBuffers> VKCommandBuffers::InternalMake(
	const EPRef<VKCommandPool>& pVKCommandPool
) {
	RESULT r = R::OK;
	EPRef<VKCommandBuffers> pVKCommandBuffer = nullptr;

	pVKCommandBuffer = new VKCommandBuffers(pVKCommandPool);
	CNM(pVKCommandBuffer, "Failed to allocate vk command buffer");

	CRM(pVKCommandBuffer->Initialize(), "Failed to initialize VK command buffer");

Success:
	return pVKCommandBuffer;

Error:
	pVKCommandBuffer = nullptr;
	return nullptr;
}