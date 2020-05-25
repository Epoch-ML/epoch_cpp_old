#include "VKSampler.h"

#include "VulkanUtilities.h"

RESULT VKSampler::Initialize() {
	RESULT r = R::OK;

	VkSamplerCreateInfo vkSamplerCreateInfo = {};

	vkSamplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	vkSamplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	vkSamplerCreateInfo.minFilter = VK_FILTER_LINEAR;

	vkSamplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	vkSamplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	vkSamplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	vkSamplerCreateInfo.anisotropyEnable = VK_TRUE;
	vkSamplerCreateInfo.maxAnisotropy = 16.0f;

	vkSamplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	vkSamplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	vkSamplerCreateInfo.compareEnable = VK_FALSE;
	vkSamplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	vkSamplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	vkSamplerCreateInfo.mipLodBias = 0.0f;
	vkSamplerCreateInfo.minLod = 0.0f;
	vkSamplerCreateInfo.maxLod = 0.0f;

	CVKRM(vkCreateSampler(m_vkLogicalDevice, &vkSamplerCreateInfo, nullptr, &m_vkSampler),
		"Failed to create vulkan sampler");

Error:
	return r;
}

RESULT VKSampler::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	CN(m_vkSampler);

	vkDestroySampler(m_vkLogicalDevice, m_vkSampler, nullptr);
	m_vkSampler = nullptr;

Error:
	return r;
}

EPRef<VKSampler> VKSampler::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice
) {
	RESULT r = R::OK;
	EPRef<VKSampler> pVKSampler = nullptr;

	pVKSampler = new VKSampler(vkPhysicalDevice, vkLogicalDevice);
	CNM(pVKSampler, "Failed to allocate vk sampler");

	CRM(pVKSampler->Initialize(), "Failed to initialize VK sampler");

Success:
	return pVKSampler;

Error:
	pVKSampler = nullptr;
	return nullptr;
}

VKSampler::VKSampler(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice)
{
	//
}

VKSampler::~VKSampler() {
	Kill();
}