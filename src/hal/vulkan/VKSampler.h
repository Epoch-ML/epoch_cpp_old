#ifndef VULKAN_SAMPLER_H_
#define VULKAN_SAMPLER_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Sampler
// epoch/src/hal/vulkan/VKSampler.h

#include "hal/sampler.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

class VKSampler :
	public sampler,
	public EPFactoryMethod<VKSampler, VkPhysicalDevice, VkDevice>
{
private:
	VKSampler(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKSampler() override;

	static EPRef<VKSampler> InternalMake(VkPhysicalDevice, VkDevice);

	const VkSampler GetVKSamplerHandle() { return m_vkSampler; }

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	VkSampler m_vkSampler = nullptr;
};

#endif // ! VULKAN_PIPELINE_H_