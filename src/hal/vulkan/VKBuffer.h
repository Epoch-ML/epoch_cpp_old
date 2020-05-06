#ifndef VULKAN_BUFFER_H_
#define VULKAN_BUFFER_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Buffer
// epoch/src/hal/vulkan/VKBuffer.h

#include "hal/buffer.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"

#include "VulkanUtilities.h"

class VKBuffer :
	public buffer,
	public EPFactoryMethod<VKBuffer, VkPhysicalDevice, VkDevice, size_t, VkBufferUsageFlags, VkMemoryPropertyFlags>
{
protected:
	VKBuffer(
		VkPhysicalDevice vkPhysicalDevice, 
		VkDevice vkLogicalDevice, 
		size_t size, 
		VkBufferUsageFlags vkBufferUsageFlags,
		VkMemoryPropertyFlags vkMemoryPropertyFlags
	) :
		m_vkPhysicalDevice(vkPhysicalDevice),
		m_vkLogicalDevice(vkLogicalDevice),
		m_size(size),
		m_vkBufferUsageFlags(vkBufferUsageFlags),
		m_vkMemoryPropertyFlags(vkMemoryPropertyFlags)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKBuffer() override {
		Kill();
	}

	static RESULT CreateBuffer(
		VkPhysicalDevice vkPhysicalDevice,
		VkDevice vkLogicalDevice,
		VkDeviceSize bufferSize,
		VkBufferUsageFlags vkBufferUsageFlags,
		VkMemoryPropertyFlags vkMemoryPropertyFlags,
		VkBuffer& r_vkBuffer,
		VkDeviceMemory& r_vkDeviceMemory);

	virtual RESULT Bind() override;
	RESULT BindAsVertexBuffer(VkCommandBuffer vkCommandBuffer);

	template <typename T>
	RESULT CopyDataToBuffer(const EPVector<T> &bufferToCopy) {
		RESULT r = R::OK;

		void* pMemoryMappedData = nullptr;
		size_t byteSize = (size_t)bufferToCopy.size() * sizeof(T);

		CVKRM(vkMapMemory(m_vkLogicalDevice, m_vkBufferDeviceMemory, 0, byteSize, 0, &pMemoryMappedData),
			"Failed to map memory to pointer");

		memcpy(pMemoryMappedData, bufferToCopy.data(), byteSize);

		vkUnmapMemory(m_vkLogicalDevice, m_vkBufferDeviceMemory);

	Error:
		return r;
	}

	static EPRef<VKBuffer> InternalMake(VkPhysicalDevice, VkDevice, size_t, VkBufferUsageFlags, VkMemoryPropertyFlags);

	const VkBuffer GetVKBufferHandle() const { return m_vkBuffer; }

protected:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	size_t m_size = 0;
	
	VkBufferUsageFlags m_vkBufferUsageFlags;
	VkMemoryPropertyFlags m_vkMemoryPropertyFlags;
	
	VkBuffer m_vkBuffer = nullptr;
	VkDeviceMemory m_vkBufferDeviceMemory = nullptr;
};

#endif // ! VULKAN_FRAMEBUFFER_H_