#ifndef VULKAN_VERTEX_BUFFER_H_
#define VULKAN_VERTEX_BUFFER_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Vertex Buffer
// epoch/src/hal/vulkan/VKVertexBuffer.h

#include "hal/buffer.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VulkanUtilities.h"

class VKCommandPool;

class VKVertexBuffer : 
	public buffer,
	virtual public EPFactoryMethod<VKVertexBuffer, VkPhysicalDevice, VkDevice, EPRef<VKCommandPool>, VkQueue, size_t>
{
protected:
	VKVertexBuffer(
		VkPhysicalDevice vkPhysicalDevice, 
		VkDevice vkLogicalDevice, 
		EPRef<VKCommandPool> pVKCommandPool, 
		VkQueue vkQueue,
		size_t size);

public:
	virtual ~VKVertexBuffer() override;

	virtual RESULT Bind() override {
		return R::NOT_IMPLEMENTED;
	}

	RESULT Bind(VkCommandBuffer vkCommandBuffer);

	static EPRef<VKVertexBuffer> InternalMake(VkPhysicalDevice, VkDevice, EPRef<VKCommandPool>, VkQueue, size_t);

	// TODO: make it like... better
	RESULT InitializeAsTriangle();

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	EPRef<VKCommandPool> m_pVKCommandPool = nullptr;
	VkQueue m_vkQueue = nullptr;

	size_t m_size = 0;

// Staging Buffer
	VkBuffer m_vkStagingBuffer = nullptr;
	VkDeviceMemory m_vkStagingBufferDeviceMemory = nullptr;

// Vertex Buffer
	VkBuffer m_vkVertexBuffer = nullptr;
	VkDeviceMemory m_vkVertexBufferDeviceMemory = nullptr;
};

#endif // ! VULKAN_FRAMEBUFFER_H_