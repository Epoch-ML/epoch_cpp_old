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
	vkImageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	vkImageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	vkImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	vkImageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
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
	VkMemoryPropertyFlagBits vkMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	for (memoryTypeIndex = 0; memoryTypeIndex < vkPhysicalDeviceMemoryProperties.memoryTypeCount; memoryTypeIndex++) {
		if ((vkMemoryRequirements.memoryTypeBits & (1 << memoryTypeIndex)) &&
			(vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & vkMemoryPropertyFlags) == vkMemoryPropertyFlags)
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
	uint32_t width, uint32_t height
) {
	RESULT r = R::OK;
	EPRef<VKImage> pVKImage = nullptr;

	pVKImage = new VKImage(vkPhysicalDevice, vkLogicalDevice, width, height);
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
	uint32_t height
) :
	image(width, height),
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice)
{
	//
}

VKImage::~VKImage() {
	Kill();
}