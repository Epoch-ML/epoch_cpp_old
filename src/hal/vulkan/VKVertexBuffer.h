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
		m_size(size),
		m_vkBufferUsageFlags(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)	
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

	RESULT AllocateMemory();

	template <typename T>
	RESULT CopyDataToBuffer(const EPVector<T>& bufferToCopy) {
		RESULT r = R::OK;

		void* pMemoryMappedData = nullptr;
		//VkDeviceSize sizeToWrite = std::min(bufferToCopy.size(), m_vkMemoryRequirements.size);

		CVKRM(vkMapMemory(m_vkLogicalDevice, m_vkBufferDeviceMemory, 0, m_vkMemoryRequirements.size, 0, &pMemoryMappedData),
			"Failed to map memory to pointer");

		memcpy(pMemoryMappedData, bufferToCopy.data(), (size_t)bufferToCopy.size() * sizeof(T));

		vkUnmapMemory(m_vkLogicalDevice, m_vkBufferDeviceMemory);

	Error:
		return r;
	}

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	size_t m_size = 0;
	VkBufferUsageFlags m_vkBufferUsageFlags;
	VkMemoryPropertyFlags m_vkMemoryPropertyFlags;

	VkBufferCreateInfo m_vkBufferCreateInfo = {};
	VkBuffer m_vkBuffer = nullptr;

	// Memory
	VkMemoryRequirements m_vkMemoryRequirements = {};
	VkMemoryAllocateInfo m_vkMemoryAllocateInfo = {};
	VkDeviceMemory m_vkBufferDeviceMemory = nullptr;
};

#endif // ! VULKAN_FRAMEBUFFER_H_