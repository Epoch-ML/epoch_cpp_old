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
		VkQueue>
{
private:
	VKCommandPool(
		VkPhysicalDevice vkPhysicalDevice,
		VkDevice vkLogicalDevice,
		VkSurfaceKHR vkSurface,
		VkQueue vkQueue
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

	static EPRef<VKCommandPool> InternalMake(
		VkPhysicalDevice, 
		VkDevice, 
		VkSurfaceKHR, 
		VkQueue);

	EPRef<VKCommandBuffers> MakeVertexDescriptorCommandBuffers(
		const EPRef<VKPipeline>&,
		const EPRef<VKSwapchain>&,
		const EPRef<VKVertexBuffer>&, 
		const EPRef<VKDescriptorSet>&);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	VkSurfaceKHR m_vkSurface = nullptr;
	VkQueue m_vkQueue = nullptr;

	VkCommandPoolCreateInfo m_vkCommandPoolCreateInfo = {};
	VkCommandPool m_vkCommandPool = nullptr;
};

#endif // ! VULKAN_COMMAND_POOL_H_