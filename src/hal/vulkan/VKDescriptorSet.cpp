#include "VKDescriptorSet.h"

#include "VulkanUtilities.h"
#include "VKDescriptorPool.h"

#include "VKUniformBuffer.h"
#include "VKImageView.h"
#include "VKSampler.h"
#include "VKTexture.h"

RESULT VKDescriptorSet::Initialize() {
	RESULT r = R::OK;

	uint32_t swapchainImageCount = static_cast<uint32_t>(m_pVKDescriptorPool->GetVKSwapchain()->GetSwapchainImageCount());

	CNM(m_vkLogicalDevice, "Cannot initialize descriptor set without valid logical device");
	CNM(m_pVKDescriptorPool, "Cannot initialize descriptor set without valid descriptor pool");

	m_vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	m_vkDescriptorSetAllocateInfo.descriptorPool = m_pVKDescriptorPool->GetVKDescriptorPoolHandle();
	m_vkDescriptorSetAllocateInfo.descriptorSetCount = swapchainImageCount;
	m_vkDescriptorSetAllocateInfo.pSetLayouts = m_vkDescriptorSetLayouts.data();
	
	// Allocate the descriptor sets
	m_vkDescriptorSets = EPVector<VkDescriptorSet>(swapchainImageCount, true);
	CVKRM(vkAllocateDescriptorSets(m_vkLogicalDevice, &m_vkDescriptorSetAllocateInfo, m_vkDescriptorSets.data()),
		"Failed to allocate descriptor sets");

	// TODO: Make this more general bruv
	for (size_t i = 0; i < swapchainImageCount; i++) {
		VkDescriptorBufferInfo vkDescriptionBufferInfo = {};
		vkDescriptionBufferInfo.buffer = m_pVKUniformBuffer->GetUniformBuffer((uint32_t)(i)),
		vkDescriptionBufferInfo.offset = 0;
		vkDescriptionBufferInfo.range = m_pVKUniformBuffer->GetUniforBufferObjectSize();

		VkDescriptorImageInfo vkDescriptorImageInfo = {};
		vkDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		vkDescriptorImageInfo.imageView = m_pVKTexture->GetVKImageView()->GetVKImageViewHandle();
		vkDescriptorImageInfo.sampler = m_pVKTexture->GetVKSampler()->GetVKSamplerHandle();

		EPArray<VkWriteDescriptorSet, 2> vkWriteDescriptorSets;

		vkWriteDescriptorSets[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vkWriteDescriptorSets[0].dstSet = m_vkDescriptorSets[i];
		vkWriteDescriptorSets[0].dstBinding = 0;		// binding
		vkWriteDescriptorSets[0].dstArrayElement = 0;
		vkWriteDescriptorSets[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vkWriteDescriptorSets[0].descriptorCount = 1;
		vkWriteDescriptorSets[0].pBufferInfo = &vkDescriptionBufferInfo;
		vkWriteDescriptorSets[0].pImageInfo = nullptr; // Optional
		vkWriteDescriptorSets[0].pTexelBufferView = nullptr; // Optional

		vkWriteDescriptorSets[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vkWriteDescriptorSets[1].dstSet = m_vkDescriptorSets[i];
		vkWriteDescriptorSets[1].dstBinding = 1;		// binding
		vkWriteDescriptorSets[1].dstArrayElement = 0;
		vkWriteDescriptorSets[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		vkWriteDescriptorSets[1].descriptorCount = 1;
		vkWriteDescriptorSets[1].pBufferInfo = nullptr;
		vkWriteDescriptorSets[1].pImageInfo = &vkDescriptorImageInfo; // Optional
		vkWriteDescriptorSets[1].pTexelBufferView = nullptr; // Optional

		// Update the descriptor set
		vkUpdateDescriptorSets(
			m_vkLogicalDevice, 
			static_cast<uint32_t>(vkWriteDescriptorSets.size()), 
			vkWriteDescriptorSets.data, 
			0, 
			nullptr
		);
	}

Error:
	return r;
}

RESULT VKDescriptorSet::Bind(VkCommandBuffer vkCommandBuffer, VkPipelineLayout vkPipelineLayout, uint32_t index) {

	vkCmdBindDescriptorSets(
		vkCommandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		vkPipelineLayout,
		0,
		1,
		&m_vkDescriptorSets[index], 0, nullptr
	);

	return R::RETURN_VOID;
}

RESULT VKDescriptorSet::Kill() {
	RESULT r = R::OK;

	// Don't need to destroy descriptor sets - will be done by the pool
	// automatically

Error:
	return r;
}

EPRef<VKDescriptorSet> VKDescriptorSet::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKDescriptorPool>& pVKDescriptorPool,
	const EPRef<VKUniformBuffer>& pVKUniformBuffer,
	const EPRef<VKTexture>& pVKTexture,
	VkDescriptorSetLayout vkDescriptorSetLayout
) {
	RESULT r = R::OK;
	EPRef<VKDescriptorSet> pVKDescriptorSet = nullptr;

	pVKDescriptorSet = new VKDescriptorSet(
		vkPhysicalDevice, 
		vkLogicalDevice, 
		pVKDescriptorPool,
		pVKUniformBuffer,
		pVKTexture,
		vkDescriptorSetLayout
	);
	CNM(pVKDescriptorSet, "Failed to allocate vk descriptor set");

	CRM(pVKDescriptorSet->Initialize(), "Failed to initialize VK descriptor set");

Success:
	return pVKDescriptorSet;

Error:
	pVKDescriptorSet = nullptr;
	return nullptr;
}

VKDescriptorSet::VKDescriptorSet(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKDescriptorPool>& pVKDescriptorPool,
	const EPRef<VKUniformBuffer>& pVKUniformBuffer,
	const EPRef<VKTexture>& pVKTexture,
	VkDescriptorSetLayout vkDescriptorSetLayout
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKUniformBuffer(pVKUniformBuffer),
	m_pVKTexture(pVKTexture),
	m_pVKDescriptorPool(pVKDescriptorPool)
{
	m_vkDescriptorSetLayouts = EPVector<VkDescriptorSetLayout>(
		m_pVKDescriptorPool->GetVKSwapchain()->GetSwapchainImageCount(),
		vkDescriptorSetLayout);
}

VKDescriptorSet::~VKDescriptorSet() {
	Kill();
}