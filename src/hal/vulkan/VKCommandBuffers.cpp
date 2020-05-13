#include "VKCommandBuffers.h"

#include "VulkanUtilities.h"

#include "VKCommandPool.h"

#include "VKVertex.h"
#include "VKBuffer.h"
#include "VKVertexBuffer.h"
#include "VKDescriptorSet.h"

VKCommandBuffers::VKCommandBuffers(
	const EPRef<VKCommandPool>& pVKCommandPool, 
	const EPRef<VKVertexBuffer>& pVKVertexBuffer, 
	const EPRef<VKDescriptorSet>& pVKDescriptorSet
) :
	m_pVKCommandPool(pVKCommandPool),
	m_pVKVertexBuffer(pVKVertexBuffer),
	m_pVKDescriptorSet(pVKDescriptorSet)
{
	//
}

VKCommandBuffers::~VKCommandBuffers() {
	Kill();
}

RESULT VKCommandBuffers::Initialize() {
	RESULT r = R::OK;

	CN(m_pVKCommandPool);
	
	m_vkCommandBuffers = EPVector<VkCommandBuffer>(m_pVKCommandPool->GetVKSwapchain()->GetFramebufferCount(), true);

	m_vkCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_vkCommandBufferAllocateInfo.commandPool = m_pVKCommandPool->GetVKCommandPoolHandle();
	m_vkCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_vkCommandBufferAllocateInfo.commandBufferCount = (uint32_t)m_vkCommandBuffers.size();

	CVKRM(vkAllocateCommandBuffers(m_pVKCommandPool->GetVKLogicalDeviceHandle(), &m_vkCommandBufferAllocateInfo, m_vkCommandBuffers.data()),
		"Failed to allocate command buffers");

	CRM(RecordCommandBuffers(), "Failed to record command buffers");

Error:
	return r;
}

RESULT VKCommandBuffers::Kill() {
	RESULT r = R::OK;

	CN(m_pVKCommandPool);

	m_pVKVertexBuffer = nullptr;

	m_pVKDescriptorSet = nullptr;

	m_pVKCommandPool = nullptr;

Error:
	return r;
}

EPRef<VKCommandBuffers> VKCommandBuffers::InternalMake(
	const EPRef<VKCommandPool>& pVKCommandPool,
	const EPRef<VKVertexBuffer>& pVKVertexBuffer,
	const EPRef<VKDescriptorSet>& pVKDescriptorSet
) {
	RESULT r = R::OK;
	EPRef<VKCommandBuffers> pVKCommandBuffer = nullptr;

	pVKCommandBuffer = new VKCommandBuffers(pVKCommandPool, pVKVertexBuffer, pVKDescriptorSet);
	CNM(pVKCommandBuffer, "Failed to allocate vk command buffer");

	CRM(pVKCommandBuffer->Initialize(), "Failed to initialize VK command buffer");

Success:
	return pVKCommandBuffer;

Error:
	pVKCommandBuffer = nullptr;
	return nullptr;
}

RESULT VKCommandBuffers::RecordCommandBuffers() {
	RESULT r = R::OK;

	VKQueueFamilies vkQueueFamilies;

	CNM(m_pVKVertexBuffer, "Cannot record command buffers without a vertex buffer");
	CNM(m_pVKDescriptorSet, "Cannot record command buffers without a descriptor set");

	vkQueueFamilies = FindQueueFamilies(
		m_pVKCommandPool->GetVKPhyscialDeviceHandle(),
		m_pVKCommandPool->GetVKSurfaceHandle()
	);

	uint32_t graphicsPipeline = vkQueueFamilies.GetGraphicsQueueIndex();

	for (uint32_t i = 0; i < m_vkCommandBuffers.size(); i++) {
		VkCommandBufferBeginInfo vkCommandBufferBeginInfo = {};

		vkCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkCommandBufferBeginInfo.flags = 0; // Optional
		vkCommandBufferBeginInfo.pInheritanceInfo = nullptr; // Optional

		CVKRM(vkBeginCommandBuffer(m_vkCommandBuffers[i], &vkCommandBufferBeginInfo),
			"Failed to begin command buffer recording");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_pVKCommandPool->GetVKPipeline()->GetVKRenderPassHandle();
		renderPassInfo.framebuffer = m_pVKCommandPool->GetVKSwapchain()->GetSwapchainFramebuffers(i);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_pVKCommandPool->GetVKSwapchain()->GetSwapchainExtent();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(m_vkCommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// Bind the pipeline
		vkCmdBindPipeline(
			m_vkCommandBuffers[i], 
			VK_PIPELINE_BIND_POINT_GRAPHICS, 
			m_pVKCommandPool->GetVKPipeline()->GetVKPipelineHandle()
		);

		//// TODO: wtf land - 
		// a lot of arch needs to go into this
		m_pVKVertexBuffer->Bind(m_vkCommandBuffers[i]);
		m_pVKDescriptorSet->Bind(m_vkCommandBuffers[i], m_pVKCommandPool->GetVKPipeline()->GetVKPipelineLayout(), i);

		m_pVKVertexBuffer->DrawIndexed(m_vkCommandBuffers[i]);

		vkCmdEndRenderPass(m_vkCommandBuffers[i]);

		CVKRM(vkEndCommandBuffer(m_vkCommandBuffers[i]),
			"Failed to end command buffer recording");
	}

Error:
	return r;
}