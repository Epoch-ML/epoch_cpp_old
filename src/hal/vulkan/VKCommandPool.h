#ifndef VULKAN_COMMAND_POOL_H_
#define VULKAN_COMMAND_POOL_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Framebuffer
// epoch/src/hal/vulkan/VKFramebuffer.h

#include "hal/command_pool.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"

class VKCommandPool :
	public command_pool,
	public EPFactoryMethod<VKCommandPool, VkDevice, const EPRef<VKPipeline>&, const EPRef<VKSwapchain>&>
{
private:
	VKCommandPool(VkDevice vkLogicalDevice, const EPRef<VKPipeline>& pVKPipeline, const EPRef<VKSwapchain>& pVKSwapchain) :
		m_vkLogicalDevice(vkLogicalDevice),
		m_pVKPipeline(pVKPipeline),
		m_pVKSwapchain(pVKSwapchain)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKCommandPool() override {
		Kill();
	}

	static EPRef<VKCommandPool> InternalMake(VkDevice, const EPRef<VKPipeline>&, const EPRef<VKSwapchain>&);

private:
	VkDevice m_vkLogicalDevice = nullptr;
	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;
	EPRef<VKPipeline> m_pVKPipeline = nullptr;


};

#endif // ! VULKAN_COMMAND_POOL_H_