#ifndef VULKAN_FRAMEBUFFER_H_
#define VULKAN_FRAMEBUFFER_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Framebuffer
// epoch/src/hal/vulkan/VKFramebuffer.h

#include "hal/framebuffer.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"

class VKFramebuffer :
	public framebuffer,
	public EPFactoryMethod<VKFramebuffer, VkDevice, const EPRef<VKPipeline>&, const EPRef<VKSwapchain>&>
{
private:
	VKFramebuffer(VkDevice vkLogicalDevice, const EPRef<VKPipeline>& pVKPipeline, const EPRef<VKSwapchain>& pVKSwapchain) :
		m_vkLogicalDevice(vkLogicalDevice),
		m_pVKPipeline(pVKPipeline),
		m_pVKSwapchain(pVKSwapchain)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKFramebuffer() override {
		Kill();
	}

	static EPRef<VKFramebuffer> InternalMake(VkDevice, const EPRef<VKPipeline>&, const EPRef<VKSwapchain>&);

private:
	VkDevice m_vkLogicalDevice = nullptr;

	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;
	EPRef<VKPipeline> m_pVKPipeline = nullptr;

	// Framebuffer
	VkFramebufferCreateInfo m_vkFramebufferCreateInfo = {};
	VkFramebuffer m_vkFramebuffer;
};

#endif // ! VULKAN_FRAMEBUFFER_H_