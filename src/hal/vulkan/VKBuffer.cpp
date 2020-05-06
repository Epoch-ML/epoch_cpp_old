#include "VKBuffer.h"

#include "VulkanUtilities.h"

RESULT VKBuffer::Initialize() {
	RESULT r = R::OK;

	CNM(m_vkLogicalDevice, "Cannot initialize framebuffer without valid logical device");

	m_vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	m_vkBufferCreateInfo.size = (uint64_t)(m_size);
	m_vkBufferCreateInfo.usage = m_vkBufferUsageFlags;
	m_vkBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CVKRM(vkCreateBuffer(m_vkLogicalDevice, &m_vkBufferCreateInfo, nullptr, &m_vkBuffer),
		"Failed to create vk buffer");
	CNM(m_vkBuffer, "Failed to create vk buffer");

	CRM(Allocate(), "Failed to allocate buffer");

Error:
	return r;
}

RESULT VKBuffer::Allocate() {
	RESULT r = R::OK;
	VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties = {};

	VkMemoryPropertyFlags vkMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

	CN(m_vkBuffer);
	
	// Get memory requirements and physical device memory properties 
	vkGetBufferMemoryRequirements(m_vkLogicalDevice, m_vkBuffer, &m_vkMemoryRequirements);
	vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &vkPhysicalDeviceMemoryProperties);

	// Find suitable memory type
	uint32_t memoryTypeIndex;
	bool fFound = false;
	for (memoryTypeIndex = 0; memoryTypeIndex < vkPhysicalDeviceMemoryProperties.memoryTypeCount; memoryTypeIndex++) {
		if ((m_vkMemoryRequirements.memoryTypeBits & (1 << memoryTypeIndex)) &&
			(vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & vkMemoryPropertyFlags) == vkMemoryPropertyFlags)
		{
			fFound = true;
			break;
		}
	}

	CBM(fFound, "Failed to find suitable memory type");

	// Allocate 
	m_vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_vkMemoryAllocateInfo.allocationSize = m_vkMemoryRequirements.size;
	m_vkMemoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

	CVKRM(vkAllocateMemory(m_vkLogicalDevice, &m_vkMemoryAllocateInfo, nullptr, &m_vkBufferDeviceMemory),
		"Failed to allocate buffer memory");
	CNM(m_vkBufferDeviceMemory, "Failed to allocate buffer memory");

	CVKRM(vkBindBufferMemory(m_vkLogicalDevice, m_vkBuffer, m_vkBufferDeviceMemory, 0),
		"Failed to bing memory to buffer");

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

EPRef<VKBuffer> VKBuffer::InternalMake(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice, size_t size, VkBufferUsageFlags vkBufferUsageFlags) {
	RESULT r = R::OK;
	EPRef<VKBuffer> pVKBuffer = nullptr;

	pVKBuffer = new VKBuffer(vkPhysicalDevice, vkLogicalDevice, size, vkBufferUsageFlags);
	CNM(pVKBuffer, "Failed to allocate vk buffer");

	CRM(pVKBuffer->Initialize(), "Failed to initialize VK buffer");

Success:
	return pVKBuffer;

Error:
	pVKBuffer = nullptr;
	return nullptr;
}