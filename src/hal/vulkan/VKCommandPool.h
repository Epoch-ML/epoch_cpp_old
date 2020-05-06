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
#include "VKCommandBuffers.h"

class VKCommandPool :
	public command_pool,
	public EPFactoryMethod<VKCommandPool, 
		VkPhysicalDevice, 
		VkDevice, 
		VkSurfaceKHR, 
		VkQueue,
		const EPRef<VKPipeline>&, 
		const EPRef<VKSwapchain>&>
{
private:
	VKCommandPool(
		VkPhysicalDevice vkPhysicalDevice, 
		VkDevice vkLogicalDevice, 
		VkSurfaceKHR vkSurface, 
		VkQueue vkQueue,
		const EPRef<VKPipeline>& pVKPipeline, 
		const EPRef<VKSwapchain>& pVKSwapchain
	) :
		m_vkPhysicalDevice(vkPhysicalDevice),
		m_vkLogicalDevice(vkLogicalDevice),
		m_vkSurface(vkSurface),
		m_vkQueue(vkQueue),
		m_pVKPipeline(pVKPipeline),
		m_pVKSwapchain(pVKSwapchain)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

	RESULT InitializeCommandBuffers();

public:
	virtual ~VKCommandPool() override {
		Kill();
	}

	VkCommandPool GetVKCommandPoolHandle() { return m_vkCommandPool; }
	VkPhysicalDevice GetVKPhyscialDeviceHandle() { return m_vkPhysicalDevice; }
	VkDevice GetVKLogicalDeviceHandle() { return m_vkLogicalDevice; }
	VkSurfaceKHR GetVKSurfaceHandle() { return m_vkSurface; }
	VkQueue GetVKQueueHandle() { return m_vkQueue; }

	const EPRef<VKSwapchain>& GetVKSwapchain() const { return m_pVKSwapchain; }
	const EPRef<VKPipeline>& GetVKPipeline() const { return m_pVKPipeline; }

	const VkCommandBuffer* GetCommandBufferHandle(uint32_t index) const { 
		if (m_pVKCommandBuffers == nullptr)
			return nullptr;

		return m_pVKCommandBuffers->GetCommandBufferHandle(index); 
	}

	static EPRef<VKCommandPool> InternalMake(
		VkPhysicalDevice, 
		VkDevice, 
		VkSurfaceKHR, 
		VkQueue,
		const EPRef<VKPipeline>&, 
		const EPRef<VKSwapchain>&);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	VkSurfaceKHR m_vkSurface = nullptr;
	VkQueue m_vkQueue = nullptr;

	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;
	EPRef<VKPipeline> m_pVKPipeline = nullptr;

	VkCommandPoolCreateInfo m_vkCommandPoolCreateInfo = {};
	VkCommandPool m_vkCommandPool = nullptr;

	EPRef<VKCommandBuffers> m_pVKCommandBuffers = nullptr;
};

#endif // ! VULKAN_COMMAND_POOL_H_