#ifndef VULKAN_DESCRIPTOR_SET_H_
#define VULKAN_DESCRIPTOR_SET_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Descriptor Set
// epoch/src/hal/vulkan/VKDescriptorSet.h

#include "core/types/EPObj.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"

class VKDescriptorPool;

class VKUniformBuffer;
class VKImageView;
class VKSampler;

// TODO: 
// - layout(set = 0, binding = 0) uniform UniformBufferObject { ... }
// bind multiple descriptor sets at the same time

class VKDescriptorSet :
	public EPObj,
	public EPFactoryMethod<VKDescriptorSet,
	VkPhysicalDevice,
	VkDevice,
	const EPRef<VKDescriptorPool>&,
	const EPRef<VKUniformBuffer>&,
	//const EPRef<VKImageView>&,
	//const EPRef<VKSampler>&,
	VkDescriptorSetLayout>
{
private:
	VKDescriptorSet(
		VkPhysicalDevice vkPhysicalDevice,
		VkDevice vkLogicalDevice,
		const EPRef<VKDescriptorPool>& pVKDescriptorPool,
		const EPRef<VKUniformBuffer>& pVKUniformBuffer,
		//const EPRef<VKImageView>& pVKImageView,
		//const EPRef<VKSampler>& pVKSampler,
		VkDescriptorSetLayout vkDescriptorSetLayout
	);

	virtual RESULT Initialize();
	virtual RESULT Kill();

public:
	virtual ~VKDescriptorSet() override;

public:
	static EPRef<VKDescriptorSet> InternalMake(
		VkPhysicalDevice,
		VkDevice,
		const EPRef<VKDescriptorPool>&,
		const EPRef<VKUniformBuffer>&,
		//const EPRef<VKImageView>&,
		//const EPRef<VKSampler>&,
		VkDescriptorSetLayout);

	RESULT Bind(VkCommandBuffer vkCommandBuffer, VkPipelineLayout vkPipelineLayout, uint32_t index);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	EPRef<VKDescriptorPool> m_pVKDescriptorPool = nullptr;

	// TODO: this is not general
	EPRef<VKUniformBuffer> m_pVKUniformBuffer = nullptr;	
	//EPRef<VKImageView> m_pVKImageView = nullptr;
	//EPRef<VKSampler> m_pVKSampler = nullptr;

	VkDescriptorSetAllocateInfo m_vkDescriptorSetAllocateInfo = {};
	EPVector<VkDescriptorSetLayout> m_vkDescriptorSetLayouts;
	EPVector<VkDescriptorSet> m_vkDescriptorSets;

};

#endif // ! VULKAN_COMMAND_POOL_H_