#ifndef VULKAN_COMMAND_POOL_H_
#define VULKAN_COMMAND_POOL_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Framebuffer
// epoch/src/hal/vulkan/VKFramebuffer.h

#include "hal/pool.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

class VKSwapchain;
class VKPipeline;
class VKCommandBuffers;
class VKVertexBuffer;
class VKDescriptorSet;

class VKCommandPool :
	public pool,
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
	);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKCommandPool() override;

	VkCommandPool GetVKCommandPoolHandle() { return m_vkCommandPool; }
	VkPhysicalDevice GetVKPhyscialDeviceHandle() { return m_vkPhysicalDevice; }
	VkDevice GetVKLogicalDeviceHandle() { return m_vkLogicalDevice; }
	VkSurfaceKHR GetVKSurfaceHandle() { return m_vkSurface; }
	VkQueue GetVKQueueHandle() { return m_vkQueue; }

	const EPRef<VKSwapchain>& GetVKSwapchain() const { return m_pVKSwapchain; }
	const EPRef<VKPipeline>& GetVKPipeline() const { return m_pVKPipeline; }

	static EPRef<VKCommandPool> InternalMake(
		VkPhysicalDevice, 
		VkDevice, 
		VkSurfaceKHR, 
		VkQueue,
		const EPRef<VKPipeline>&, 
		const EPRef<VKSwapchain>&);

	EPRef<VKCommandBuffers> MakeCommandBuffers(const EPRef<VKVertexBuffer>&, const EPRef<VKDescriptorSet>&);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	VkSurfaceKHR m_vkSurface = nullptr;
	VkQueue m_vkQueue = nullptr;

	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;
	EPRef<VKPipeline> m_pVKPipeline = nullptr;

	VkCommandPoolCreateInfo m_vkCommandPoolCreateInfo = {};
	VkCommandPool m_vkCommandPool = nullptr;
};

#endif // ! VULKAN_COMMAND_POOL_H_