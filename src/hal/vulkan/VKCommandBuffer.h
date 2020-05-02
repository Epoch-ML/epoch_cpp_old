#ifndef VULKAN_COMMAND_BUFFER_H_
#define VULKAN_COMMAND_BUFFER_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Framebuffer
// epoch/src/hal/vulkan/VKFramebuffer.h

#include "hal/command_buffer.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"

class VKCommandBuffer :
	public command_buffer,
	public EPFactoryMethod<VKCommandBuffer, VkPhysicalDevice, VkDevice, VkSurfaceKHR, const EPRef<VKPipeline>&, const EPRef<VKSwapchain>&>
{
private:
	VKCommandBuffer(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice, VkSurfaceKHR vkSurface, const EPRef<VKPipeline>& pVKPipeline, const EPRef<VKSwapchain>& pVKSwapchain) :
		m_vkPhysicalDevice(vkPhysicalDevice),
		m_vkLogicalDevice(vkLogicalDevice),
		m_vkSurface(vkSurface),
		m_pVKPipeline(pVKPipeline),
		m_pVKSwapchain(pVKSwapchain)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKCommandBuffer() override {
		Kill();
	}

	static EPRef<VKCommandBuffer> InternalMake(VkPhysicalDevice, VkDevice, VkSurfaceKHR, const EPRef<VKPipeline>&, const EPRef<VKSwapchain>&);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	VkSurfaceKHR m_vkSurface = nullptr;

	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;
	EPRef<VKPipeline> m_pVKPipeline = nullptr;

	VkCommandPoolCreateInfo m_vkCommandPoolCreateInfo = {};
	VkCommandPool m_vkCommandPool = nullptr;
};

#endif // ! VULKAN_COMMAND_BUFFER_H_