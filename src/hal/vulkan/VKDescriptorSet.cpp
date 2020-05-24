#include "VKDescriptorSet.h"

#include "VulkanUtilities.h"
#include "VKDescriptorPool.h"

#include "VKUniformBuffer.h"
#include "VKImageView.h"
#include "VKSampler.h"

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

		//VkDescriptorImageInfo vkDescriptormageInfo = {};
		//vkDescriptormageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//vkDescriptormageInfo.imageView = m_pVKImageView->GetVKImageViewHandle();
		//vkDescriptormageInfo.sampler = m_pVKSampler->GetVKSamplerHandle();

		VkWriteDescriptorSet vkWriteDescriptorSet = {};
		vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vkWriteDescriptorSet.dstSet = m_vkDescriptorSets[i];
		vkWriteDescriptorSet.dstBinding = 0;		// binding
		vkWriteDescriptorSet.dstArrayElement = 0;
		vkWriteDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vkWriteDescriptorSet.descriptorCount = 1;
		vkWriteDescriptorSet.pBufferInfo = &vkDescriptionBufferInfo;
		vkWriteDescriptorSet.pImageInfo = nullptr; // Optional
		vkWriteDescriptorSet.pTexelBufferView = nullptr; // Optional

		// Update the descriptor set
		vkUpdateDescriptorSets(m_vkLogicalDevice, 1, &vkWriteDescriptorSet, 0, nullptr);
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
	//const EPRef<VKImageView>& pVKImageView,
	//const EPRef<VKSampler>& pVKSampler,
	VkDescriptorSetLayout vkDescriptorSetLayout
) {
	RESULT r = R::OK;
	EPRef<VKDescriptorSet> pVKDescriptorSet = nullptr;

	pVKDescriptorSet = new VKDescriptorSet(
		vkPhysicalDevice, 
		vkLogicalDevice, 
		pVKDescriptorPool,
		pVKUniformBuffer,
		//pVKImageView,
		//pVKSampler,
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
	//const EPRef<VKImageView>& pVKImageView,
	//const EPRef<VKSampler>& pVKSampler,
	VkDescriptorSetLayout vkDescriptorSetLayout
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKUniformBuffer(pVKUniformBuffer),
	//m_pVKImageView(pVKImageView),
	//m_pVKSampler(pVKSampler),
	m_pVKDescriptorPool(pVKDescriptorPool)
{
	m_vkDescriptorSetLayouts = EPVector<VkDescriptorSetLayout>(
		m_pVKDescriptorPool->GetVKSwapchain()->GetSwapchainImageCount(),
		vkDescriptorSetLayout);
}

VKDescriptorSet::~VKDescriptorSet() {
	Kill();
}