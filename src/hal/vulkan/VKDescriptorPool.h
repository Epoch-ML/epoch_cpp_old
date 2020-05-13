#ifndef VULKAN_DESCRIPTOR_POOL_H_
#define VULKAN_DESCRIPTOR_POOL_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Descriptor Pool
// epoch/src/hal/vulkan/VKDescriptorPool.h

#include "hal/pool.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"
#include "VKCommandBuffers.h"

class VKDescriptorPool :
	public pool,
	public EPFactoryMethod< VKDescriptorPool,
	VkPhysicalDevice,
	VkDevice,
	const EPRef<VKSwapchain>&>
{
private:
	VKDescriptorPool(
		VkPhysicalDevice vkPhysicalDevice,
		VkDevice vkLogicalDevice,
		const EPRef<VKSwapchain>& pVKSwapchain
	) :
		m_vkPhysicalDevice(vkPhysicalDevice),
		m_vkLogicalDevice(vkLogicalDevice),
		m_pVKSwapchain(pVKSwapchain)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKDescriptorPool() override {
		Kill();
	}

	const EPRef<VKSwapchain>& GetVKSwapchain() const { return m_pVKSwapchain; }

	static EPRef<VKDescriptorPool> InternalMake(
		VkPhysicalDevice,
		VkDevice,
		const EPRef<VKSwapchain>&);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	
	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;

	VkDescriptorPoolSize m_vkDescriptorPoolSize = {};
	VkDescriptorPoolCreateInfo m_vkDescriptorPoolCreateInfo = {};

	VkDescriptorPool m_vkDescriptorPool = nullptr;
};

#endif // ! VULKAN_COMMAND_POOL_H_