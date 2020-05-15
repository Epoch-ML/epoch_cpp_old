#include "VKImage.h"

RESULT VKImage::Initialize() {
	RESULT r = R::OK;

	// Create the VK image
	VkImageCreateInfo vkImageCreateInfo = {};
	vkImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	vkImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	vkImageCreateInfo.extent.width = m_width;
	vkImageCreateInfo.extent.height = m_height;
	vkImageCreateInfo.extent.depth = 1;
	vkImageCreateInfo.mipLevels = 1;
	vkImageCreateInfo.arrayLayers = 1;
	vkImageCreateInfo.format = m_vkFormat;
	vkImageCreateInfo.tiling = m_vkImageTiling;
	vkImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vkImageCreateInfo.usage = m_vkImageUsageFlags;
	vkImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	vkImageCreateInfo.flags = 0;	// optional

	CVKRM(vkCreateImage(m_vkLogicalDevice, &vkImageCreateInfo, nullptr, &m_vkTextureImage),
		"Failed to create image");

	VkMemoryRequirements vkMemoryRequirements = {};
	VkPhysicalDeviceMemoryProperties vkPhysicalDeviceMemoryProperties = {};
	vkGetImageMemoryRequirements(m_vkLogicalDevice, m_vkTextureImage, &vkMemoryRequirements);
	vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &vkPhysicalDeviceMemoryProperties);

	// Find suitable memory type
	uint32_t memoryTypeIndex;
	bool fFoundSuitableMemoryType = false;

	for (memoryTypeIndex = 0; memoryTypeIndex < vkPhysicalDeviceMemoryProperties.memoryTypeCount; memoryTypeIndex++) {
		if ((vkMemoryRequirements.memoryTypeBits & (1 << memoryTypeIndex)) &&
			(vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & m_vkMemoryPropertyFlags) == m_vkMemoryPropertyFlags)
		{
			fFoundSuitableMemoryType = true;
			break;
		}
	}

	CBM(fFoundSuitableMemoryType, "Failed to find suitable memory type");

	VkMemoryAllocateInfo vkMemoryAllocateInfo = {};
	vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;
	vkMemoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

	CVKRM(vkAllocateMemory(m_vkLogicalDevice, &vkMemoryAllocateInfo, nullptr, &m_vkTextureDeviceMemory),
		"Failed to allocate texture image device memory");

	// TODO: Move into other call?
	CVKRM(vkBindImageMemory(m_vkLogicalDevice, m_vkTextureImage, m_vkTextureDeviceMemory, 0),
		"Failed to bind image memory");



Error:
	return r;
}

RESULT VKImage::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);

Error:
	return r;
}

EPRef<VKImage> VKImage::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	uint32_t width, uint32_t height,
	VkFormat vkFormat,
	VkImageTiling vkImageTiling,
	VkImageUsageFlags vkImageUsageFlags,
	VkMemoryPropertyFlags vkMemoryPropertyFlags
) {
	RESULT r = R::OK;
	EPRef<VKImage> pVKImage = nullptr;

	pVKImage = new VKImage(
		vkPhysicalDevice, 
		vkLogicalDevice, 
		width, height, 
		vkFormat, 
		vkImageTiling, 
		vkImageUsageFlags, 
		vkMemoryPropertyFlags
	);
	CNM(pVKImage, "Failed to allocate vk image");

	CRM(pVKImage->Initialize(), "Failed to initialize VK image");

Success:
	return pVKImage;

Error:
	pVKImage = nullptr;
	return nullptr;
}

VKImage::VKImage(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	uint32_t width, 
	uint32_t height,
	VkFormat vkFormat,
	VkImageTiling vkImageTiling,
	VkImageUsageFlags vkImageUsageFlags,
	VkMemoryPropertyFlags vkMemoryPropertyFlags
) :
	image(width, height),
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_vkFormat(vkFormat),
	m_vkImageTiling(vkImageTiling),
	m_vkImageUsageFlags(vkImageUsageFlags),
	m_vkMemoryPropertyFlags(vkMemoryPropertyFlags)
{
	//
}

VKImage::~VKImage() {
	Kill();
}