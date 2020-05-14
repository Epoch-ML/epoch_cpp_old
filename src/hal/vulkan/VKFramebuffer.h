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

class VKSwapchain;
class VKPipeline;

class VKFramebuffer :
	public framebuffer,
	public EPFactoryMethod<VKFramebuffer, VkDevice, const EPRef<VKPipeline>&, const EPRef<VKSwapchain>&, uint32_t>
{
private:
	VKFramebuffer(
		VkDevice vkLogicalDevice,
		const EPRef<VKPipeline>& pVKPipeline,
		const EPRef<VKSwapchain>& pVKSwapchain,
		uint32_t frameBufferIndex
	);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKFramebuffer() override;

	static EPRef<VKFramebuffer> InternalMake(VkDevice, const EPRef<VKPipeline>&, const EPRef<VKSwapchain>&, uint32_t);
	const VkFramebuffer GetVKFrameBufferHandle() const { return m_vkFramebuffer; }

private:
	VkDevice m_vkLogicalDevice = nullptr;

	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;
	EPRef<VKPipeline> m_pVKPipeline = nullptr;

	// Framebuffer
	uint32_t m_frameBufferIndex = 0;
	VkFramebufferCreateInfo m_vkFramebufferCreateInfo = {};
	VkFramebuffer m_vkFramebuffer = nullptr;
};

#endif // ! VULKAN_FRAMEBUFFER_H_