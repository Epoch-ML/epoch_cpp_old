#include "VKTexture.h"

#include "VKBuffer.h"
#include "VKImage.h"

RESULT VKTexture::Initialize() {
	RESULT r = R::OK;

	// Load image into staging Buffer
	CVKRM(VKBuffer::CreateBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_pImage->byte_size(),
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		m_vkStagingBuffer,
		m_vkStagingBufferDeviceMemory),
		"Failed to Create Vertex Buffer");
	CN(m_vkStagingBuffer);
	CN(m_vkStagingBufferDeviceMemory);

	CRM(VKBuffer::CopyDataToBuffer(
		m_vkPhysicalDevice,
		m_vkLogicalDevice,
		m_vkStagingBufferDeviceMemory,
		m_pImage),
		"Failed to copy vertex data");

	// This is optional here, but we can delete the image from mem now
	m_pImage = nullptr;

	m_pVKImage = VKImage::make(m_vkPhysicalDevice, m_vkLogicalDevice, m_width, m_height);
	CNM(m_pVKImage, "Failed to create VKImage");

	

	// what now

	vkDestroyBuffer(m_vkLogicalDevice, m_vkStagingBuffer, nullptr);
	vkFreeMemory(m_vkLogicalDevice, m_vkStagingBufferDeviceMemory, nullptr);

Error:
	return r;
}

RESULT VKTexture::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);

Error:
	return r;
}

EPRef<VKTexture> VKTexture::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPString<char>& strFilename
) {
	RESULT r = R::OK;
	EPRef<VKTexture> pVKTexture = nullptr;

	pVKTexture = new VKTexture(vkPhysicalDevice, vkLogicalDevice, strFilename);
	CNM(pVKTexture, "Failed to allocate vk texture");

	CRM(pVKTexture->Initialize(), "Failed to initialize VK texture");

Success:
	return pVKTexture;

Error:
	pVKTexture = nullptr;
	return nullptr;
}

VKTexture::VKTexture(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice, 
	const EPString<char>& strFilename
) :
	texture(strFilename),
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice)
{
	//
}

VKTexture::~VKTexture() {
	Kill();
}