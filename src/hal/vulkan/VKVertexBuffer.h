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

class VKVertexBuffer : 
	public buffer,
	virtual public EPFactoryMethod<VKVertexBuffer, VkPhysicalDevice, VkDevice, size_t>
{
protected:
	VKVertexBuffer(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice, size_t size) :
		m_vkPhysicalDevice(vkPhysicalDevice),
		m_vkLogicalDevice(vkLogicalDevice),
		m_size(size)
	{
		//
	}
public:
	virtual ~VKVertexBuffer() override {
		Kill();
	}

	virtual RESULT Bind() override {
		return R::NOT_IMPLEMENTED;
	}

	RESULT Bind(VkCommandBuffer vkCommandBuffer);

	static EPRef<VKVertexBuffer> InternalMake(VkPhysicalDevice, VkDevice, size_t);

	// TODO: make it like... better
	RESULT InitializeAsTriangle();

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

	RESULT CopyDataToBuffer(void* pVufferToCopy, size_t pVufferToCopy_n);

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	size_t m_size = 0;

	VkBufferUsageFlags m_vkBufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	VkMemoryPropertyFlags m_vkMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

// Staging Buffer
	VkBuffer m_vkStagingBuffer = nullptr;
	VkDeviceMemory m_vkStagingBufferDeviceMemory = nullptr;

// Vertex Buffer
	VkBuffer m_vkVertexBuffer = nullptr;
	VkDeviceMemory m_vkVertexBufferDeviceMemory = nullptr;
};

#endif // ! VULKAN_FRAMEBUFFER_H_