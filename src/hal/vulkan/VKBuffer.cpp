#include "VKBuffer.h"

#include "VKDescriptorSet.h"

#include "hal/image.h"

RESULT VKBuffer::CreateBuffer(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	VkDeviceSize bufferSize,
	VkBufferUsageFlags vkBufferUsageFlags,
	VkMemoryPropertyFlags vkMemoryPropertyFlags,
	VkBuffer& r_vkBuffer,
	VkDeviceMemory& r_vkDeviceMemory)
{
	RESULT r = R::OK;

	VkBufferCreateInfo vkBufferCreateInfo = {};
	VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties = {};
	VkMemoryRequirements vkMemoryRequirements = {};
	VkMemoryAllocateInfo vkMemoryAllocateInfo = {};

	CNM(vkLogicalDevice, "Cannot initialize framebuffer without valid logical device");

	vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vkBufferCreateInfo.size = (uint64_t)(bufferSize);
	vkBufferCreateInfo.usage = vkBufferUsageFlags;
	vkBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CVKRM(vkCreateBuffer(vkLogicalDevice, &vkBufferCreateInfo, nullptr, &r_vkBuffer),
		"Failed to create vk buffer");
	CNM(r_vkBuffer, "Failed to create vk buffer");

	// Get memory requirements and physical device memory properties 
	vkGetBufferMemoryRequirements(vkLogicalDevice, r_vkBuffer, &vkMemoryRequirements);
	vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &vkPhysicalDeviceMemoryProperties);

	// Find suitable memory type
	uint32_t memoryTypeIndex;
	bool fFoundSuitableMemoryType = false;

	for (memoryTypeIndex = 0; memoryTypeIndex < vkPhysicalDeviceMemoryProperties.memoryTypeCount; memoryTypeIndex++) {
		if ((vkMemoryRequirements.memoryTypeBits & (1 << memoryTypeIndex)) &&
			(vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & vkMemoryPropertyFlags) == vkMemoryPropertyFlags)
		{
			fFoundSuitableMemoryType = true;
			break;
		}
	}

	CBM(fFoundSuitableMemoryType, "Failed to find suitable memory type");

	// Allocate 
	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

	CVKRM(vkAllocateMemory(vkLogicalDevice, &vkMemoryAllocateInfo, nullptr, &r_vkDeviceMemory),
		"Failed to allocate buffer memory");
	CNM(r_vkDeviceMemory, "Failed to allocate buffer memory");

	CVKRM(vkBindBufferMemory(vkLogicalDevice, r_vkBuffer, r_vkDeviceMemory, 0),
		"Failed to bing memory to buffer");

Error:
	return r;
}

RESULT VKBuffer::CopyDataToBuffer(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	VkDeviceMemory& r_vkDeviceMemory,
	void* pBufferToCopy,
	size_t pBufferToCopy_n
) {
	RESULT r = R::OK;

	void* pMemoryMappedData = nullptr;

	CVKRM(vkMapMemory(vkLogicalDevice, r_vkDeviceMemory, 0, pBufferToCopy_n, 0, &pMemoryMappedData),
		"Failed to map memory to pointer");

	memcpy(pMemoryMappedData, pBufferToCopy, pBufferToCopy_n);

	vkUnmapMemory(vkLogicalDevice, r_vkDeviceMemory);

Error:
	return r;
}

static RESULT CopyDataToBuffer(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	VkDeviceMemory& r_vkDeviceMemory,
	const EPRef<image>& pImage)
{
	return VKBuffer::CopyDataToBuffer(
		vkPhysicalDevice,
		vkLogicalDevice,
		r_vkDeviceMemory,
		(void*)(pImage->data()),
		pImage->byte_size());
}

RESULT VKBuffer::CopyBuffer(
	VkDevice vkLogicalDevice, 
	VkCommandPool vkCommandPool, 
	VkQueue vkQueue,
	VkBuffer vkBufferSource, 
	VkBuffer vkBufferDestination, 
	VkDeviceSize size
) {
	RESULT r = R::OK;

	VkCommandBuffer vkCommandBuffer = nullptr;
	VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo = {};
	VkCommandBufferBeginInfo vkCommandBufferBeginInfo{};
	VkBufferCopy vkBufferCopy{};
	VkSubmitInfo vkSubmitInfo{};

	vkCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	vkCommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	vkCommandBufferAllocateInfo.commandPool = vkCommandPool;
	vkCommandBufferAllocateInfo.commandBufferCount = 1;

	CVKRM(vkAllocateCommandBuffers(vkLogicalDevice, &vkCommandBufferAllocateInfo, &vkCommandBuffer),
		"Failed to allocate command buffer");

	vkCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vkCommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	CVKRM(vkBeginCommandBuffer(vkCommandBuffer, &vkCommandBufferBeginInfo),
		"Begin command buffer failed");

	vkBufferCopy.srcOffset = 0; // Optional
	vkBufferCopy.dstOffset = 0; // Optional
	vkBufferCopy.size = size;
	vkCmdCopyBuffer(vkCommandBuffer, vkBufferSource, vkBufferDestination, 1, &vkBufferCopy);

	CVKRM(vkEndCommandBuffer(vkCommandBuffer),
		"Failed to end command buffer");

	vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.commandBufferCount = 1;
	vkSubmitInfo.pCommandBuffers = &vkCommandBuffer;

	CVKRM(vkQueueSubmit(vkQueue, 1, &vkSubmitInfo, nullptr), "Failed to submit queue");
	CVKRM(vkQueueWaitIdle(vkQueue), "Failed to wait on queue");

	vkFreeCommandBuffers(vkLogicalDevice, vkCommandPool, 1, &vkCommandBuffer);

Error:
	return r;
}

RESULT VKBuffer::Initialize() {
	RESULT r = R::OK;

	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_size,
		m_vkBufferUsageFlags,
		m_vkMemoryPropertyFlags,
		m_vkBuffer,
		m_vkBufferDeviceMemory),
		"Failed to Create Buffer");

	CN(m_vkBuffer);
	CN(m_vkBufferDeviceMemory);

Error:
	return r;
}

RESULT VKBuffer::BindAsVertexBuffer(VkCommandBuffer vkCommandBuffer) {
	VkBuffer vkBuffers[] = { m_vkBuffer };
	VkDeviceSize vkOffsets[] = { 0 };

	vkCmdBindVertexBuffers(vkCommandBuffer, 0, 1, vkBuffers, vkOffsets);

	return R::OK;
}

RESULT VKBuffer::Bind() {
	return R::NOT_IMPLEMENTED;
}

RESULT VKBuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);
	CN(m_vkBuffer);

	vkDestroyBuffer(m_vkLogicalDevice, m_vkBuffer, nullptr);

	CN(m_vkBufferDeviceMemory);
	vkFreeMemory(m_vkLogicalDevice, m_vkBufferDeviceMemory, nullptr);

Error:
	return r;
}

EPRef<VKBuffer> VKBuffer::InternalMake(
	VkPhysicalDevice vkPhysicalDevice, 
	VkDevice vkLogicalDevice, 
	size_t size, 
	VkBufferUsageFlags vkBufferUsageFlags,
	VkMemoryPropertyFlags vkMemoryPropertyFlags
) {
	RESULT r = R::OK;
	EPRef<VKBuffer> pVKBuffer = nullptr;

	pVKBuffer = new VKBuffer(vkPhysicalDevice, vkLogicalDevice, size, vkBufferUsageFlags, vkMemoryPropertyFlags);
	CNM(pVKBuffer, "Failed to allocate vk buffer");

	CRM(pVKBuffer->Initialize(), "Failed to initialize VK buffer");

Success:
	return pVKBuffer;

Error:
	pVKBuffer = nullptr;
	return nullptr;
}