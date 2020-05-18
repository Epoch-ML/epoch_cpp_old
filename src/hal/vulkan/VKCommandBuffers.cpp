#include "VKCommandBuffers.h"

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

VKCommandBuffers::VKCommandBuffers(
	const EPRef<VKCommandPool>& pVKCommandPool
) :
	m_pVKCommandPool(pVKCommandPool)
{
	//
}

VKCommandBuffers::~VKCommandBuffers() {
	Kill();
}

// TODO: This should be the way we do stuff in the future
RESULT VKCommandBuffers::ProtoInitialize(uint32_t numBuffers) {
	RESULT r = R::OK;

	CN(m_pVKCommandPool);

	m_vkCommandBuffers = EPVector<VkCommandBuffer>(numBuffers, true);
	m_vkCommandBufferStates = EPVector<CommandBufferState>(numBuffers, CommandBufferState::UNINITIALIZED);

	// Set up queue families
	m_vkQueueFamilies = FindQueueFamilies(
		m_pVKCommandPool->GetVKPhyscialDeviceHandle(),
		m_pVKCommandPool->GetVKSurfaceHandle()
	);

	uint32_t graphicsPipeline = m_vkQueueFamilies.GetGraphicsQueueIndex();

	m_vkCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_vkCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_vkCommandBufferAllocateInfo.commandPool = m_pVKCommandPool->GetVKCommandPoolHandle();
	m_vkCommandBufferAllocateInfo.commandBufferCount = (uint32_t)m_vkCommandBuffers.size();

	CVKRM(vkAllocateCommandBuffers(m_pVKCommandPool->GetVKLogicalDeviceHandle(), &m_vkCommandBufferAllocateInfo, m_vkCommandBuffers.data()),
		"Failed to allocate command buffers");

	m_vkCommandBufferStates.SetAll(CommandBufferState::INITIALIZED);

Error:
	return r;
}

RESULT VKCommandBuffers::Initialize() {
	RESULT r = R::OK;

	CN(m_pVKCommandPool);

	size_t numBuffers = m_pVKCommandPool->GetVKSwapchain()->GetFramebufferCount();

	m_vkCommandBuffers = EPVector<VkCommandBuffer>(numBuffers, true);
	m_vkCommandBufferStates = EPVector<CommandBufferState>(numBuffers, CommandBufferState::UNINITIALIZED);

	// Set up queue families
	m_vkQueueFamilies = FindQueueFamilies(
		m_pVKCommandPool->GetVKPhyscialDeviceHandle(),
		m_pVKCommandPool->GetVKSurfaceHandle()
	);

	uint32_t graphicsPipeline = m_vkQueueFamilies.GetGraphicsQueueIndex();

	m_vkCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_vkCommandBufferAllocateInfo.commandPool = m_pVKCommandPool->GetVKCommandPoolHandle();
	m_vkCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_vkCommandBufferAllocateInfo.commandBufferCount = (uint32_t)m_vkCommandBuffers.size();

	CVKRM(vkAllocateCommandBuffers(m_pVKCommandPool->GetVKLogicalDeviceHandle(), &m_vkCommandBufferAllocateInfo, m_vkCommandBuffers.data()),
		"Failed to allocate command buffers");

	m_vkCommandBufferStates.SetAll(CommandBufferState::INITIALIZED);

	CRM(RecordCommandBuffers(), "Failed to record command buffers");

Error:
	return r;
}

RESULT VKCommandBuffers::Kill() {
	RESULT r = R::OK;

	CN(m_pVKCommandPool);

	vkFreeCommandBuffers(
		m_pVKCommandPool->GetVKLogicalDeviceHandle(), 
		m_pVKCommandPool->GetVKCommandPoolHandle(), 
		static_cast<uint32_t>(m_vkCommandBuffers.size()), 
		m_vkCommandBuffers.data());
	m_vkCommandBuffers.clear();

	m_pVKVertexBuffer = nullptr;

	m_pVKDescriptorSet = nullptr;

	m_pVKCommandPool = nullptr;

Error:
	return r;
}

/*
// TODO: The proto approach
EPRef<VKCommandBuffers> VKCommandBuffers::InternalMake(const EPRef<VKCommandPool>& pVKCommandPool) {
	RESULT r = R::OK;
	EPRef<VKCommandBuffers> pVKCommandBuffer = nullptr;

	pVKCommandBuffer = new VKCommandBuffers(pVKCommandPool);
	CNM(pVKCommandBuffer, "Failed to allocate vk command buffer");

	CRM(pVKCommandBuffer->ProtoInitialize(), "Failed to *proto* initialize VK command buffer");

Success:
	return pVKCommandBuffer;

Error:
	pVKCommandBuffer = nullptr;
	return nullptr;
}
*/

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

	CNM(m_pVKVertexBuffer, "Cannot record command buffers without a vertex buffer");
	CNM(m_pVKDescriptorSet, "Cannot record command buffers without a descriptor set");

	//m_vkQueueFamilies = FindQueueFamilies(
	//	m_pVKCommandPool->GetVKPhyscialDeviceHandle(),
	//	m_pVKCommandPool->GetVKSurfaceHandle()
	//);
	//
	//uint32_t graphicsPipeline = m_vkQueueFamilies.GetGraphicsQueueIndex();

	for (uint32_t i = 0; i < m_vkCommandBuffers.size(); i++) {
		VkCommandBufferBeginInfo vkCommandBufferBeginInfo = {};

		vkCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkCommandBufferBeginInfo.flags = 0; // Optional
		vkCommandBufferBeginInfo.pInheritanceInfo = nullptr; // Optional

		CVKRM(vkBeginCommandBuffer(m_vkCommandBuffers[i], &vkCommandBufferBeginInfo),
			"Failed to begin command buffer recording");

		m_vkCommandBufferStates[i] = RECORDING;

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

		m_vkCommandBufferStates[i] = READY;
	}

Error:
	return r;
}

RESULT VKCommandBuffers::Begin(uint32_t index) {
	RESULT r = R::OK;

	VkCommandBufferBeginInfo vkCommandBufferBeginInfo = {};

	CBM(m_vkCommandBufferStates[index] == CommandBufferState::INITIALIZED, 
		"Can't start recording buffer that's already recording");

	vkCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vkCommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;	// TODO: prob not general

	CVKRM(vkBeginCommandBuffer(m_vkCommandBuffers[index], &vkCommandBufferBeginInfo),
		"Failed to begin command buffer %d", index);

	m_vkCommandBufferStates[index] = CommandBufferState::RECORDING;

Error:
	return r;
}

RESULT VKCommandBuffers::End(uint32_t index) {
	RESULT r = R::OK;

	CBM(m_vkCommandBufferStates[index] == CommandBufferState::RECORDING,
		"Can't end recording buffer that's not already recording");

	CVKRM(vkEndCommandBuffer(m_vkCommandBuffers[index]),
		"Failed to end command buffer %d", index);

	m_vkCommandBufferStates[index] = CommandBufferState::READY;

Error:
	return r;
}

RESULT VKCommandBuffers::Submit(VkQueue vkQueue) {
	RESULT r = R::OK;

	VkSubmitInfo vkSubmitInfo = {};

	CBM(m_vkCommandBufferStates.CompareAll(CommandBufferState::READY),
		"Can't submit a buffer that's not ready");

	vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.commandBufferCount = (uint32_t)m_vkCommandBuffers.size();
	vkSubmitInfo.pCommandBuffers = m_vkCommandBuffers.data();

	vkQueueSubmit(vkQueue, 1, &vkSubmitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(vkQueue);

	m_vkCommandBufferStates.SetAll(CommandBufferState::SUBMITTED);

Error:
	return r;
}

RESULT VKCommandBuffers::Submit(VkQueue vkQueue, uint32_t index) {
	RESULT r = R::OK;

	VkSubmitInfo vkSubmitInfo = {};

	CBM(m_vkCommandBufferStates[index] == CommandBufferState::READY,
		"Can't submit a buffer that's not ready");

	vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.commandBufferCount = 1;
	vkSubmitInfo.pCommandBuffers = &m_vkCommandBuffers[index];

	vkQueueSubmit(vkQueue, 1, &vkSubmitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(vkQueue);

	m_vkCommandBufferStates[index] = CommandBufferState::SUBMITTED;

Error:
	return r;
}

RESULT VKCommandBuffers::CopyBuffer(uint32_t index, VkBuffer vkSrcBuffer, VkBuffer vkDstBuffer, VkDeviceSize vkSize) {
	RESULT r = R::OK;

	VkBufferCopy vkBufferCopy = {};

	// This requires a recording buffer broh
	CBM(m_vkCommandBufferStates[index] == CommandBufferState::RECORDING,
		"Command buffer %d not recording", index);

	vkBufferCopy.size = vkSize;

	vkCmdCopyBuffer(m_vkCommandBuffers[index], vkSrcBuffer, vkDstBuffer, 1, &vkBufferCopy);

Error:
	return r;
}

RESULT VKCommandBuffers::PipelineBarrier(uint32_t index, const VkImageMemoryBarrier &vkImageMemoryBarrier) {
	RESULT r = R::OK;

	CBM(m_vkCommandBufferStates[index] == CommandBufferState::RECORDING,
		"Command buffer %d not recording", index);

	vkCmdPipelineBarrier(
		m_vkCommandBuffers[index],
		0, // TODO 
		0, // TODO 
		0,
		0, nullptr,
		0, nullptr,
		1, 
		&vkImageMemoryBarrier
	);

Error:
	return r;
}

RESULT VKCommandBuffers::CopyBufferToImage(
	uint32_t index,
	VkBuffer vkSrcBuffer, VkImage vkDstImage,
	const VkBufferImageCopy &vkBufferImageCopy)
{
	RESULT r = R::OK;

	CBM(m_vkCommandBufferStates[index] == CommandBufferState::RECORDING,
		"Command buffer %d not recording", index);

	vkCmdCopyBufferToImage(
		m_vkCommandBuffers[index],
		vkSrcBuffer,
		vkDstImage,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,		// Expand function to automatically figure this out
		1,
		&vkBufferImageCopy);

Error:
	return r;
}