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

class VKBuffer :
	public buffer,
	public EPFactoryMethod<VKBuffer, VkPhysicalDevice, VkDevice, size_t, VkBufferUsageFlags>
{
private:
	VKBuffer(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice, size_t size, VkBufferUsageFlags vkBufferUsageFlags) :
		m_vkPhysicalDevice(vkPhysicalDevice),
		m_vkLogicalDevice(vkLogicalDevice),
		m_size(size),
		m_vkBufferUsageFlags(vkBufferUsageFlags)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;


public:
	virtual ~VKBuffer() override {
		Kill();
	}

	RESULT Allocate();

	virtual RESULT Bind() override;

	template <typename T>
	RESULT CopyDataToBuffer(const EPVector<T> &bufferToCopy) {
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

	static EPRef<VKBuffer> InternalMake(VkPhysicalDevice, VkDevice, size_t, VkBufferUsageFlags);
	const VkBuffer GetVKBufferHandle() const { return m_vkBuffer; }

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	size_t m_size = 0;
	VkBufferUsageFlags m_vkBufferUsageFlags;
	VkBufferCreateInfo m_vkBufferCreateInfo = {};
	VkBuffer m_vkBuffer = nullptr;

	// Memory
	VkMemoryRequirements m_vkMemoryRequirements = {};
	VkMemoryAllocateInfo m_vkMemoryAllocateInfo = {};
	VkDeviceMemory m_vkBufferDeviceMemory = nullptr;;
};

#endif // ! VULKAN_FRAMEBUFFER_H_