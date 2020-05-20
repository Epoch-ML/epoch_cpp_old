#include "VKTexture.h"

#include "VKBuffer.h"
#include "VKImage.h"

#include "VKCommandPool.h"
#include "VKCommandBuffers.h"

#include "hal/STBImage.h"

// TODO: wtf does this need to be everywhere?
#include "VKDescriptorSet.h"

RESULT VKTexture::CopyStagingBufferToImage() {
	RESULT r = R::OK;

	VkBufferImageCopy vkBufferImageCopy = {};

	// Create a one-time command buffer / run it
	// TODO: Convert into a more general pattern
	auto pVKCommandBuffers = new VKCommandBuffers(m_pVKCommandPool);
	CNM(pVKCommandBuffers, "Failed to make a command buffer object");
	CRM(pVKCommandBuffers->ProtoInitialize(1), "Failed to *proto* initialize command buffers");

	CRM(pVKCommandBuffers->Begin(0), "Failed to start command buffer");
	
	vkBufferImageCopy.bufferOffset = 0;
	vkBufferImageCopy.bufferRowLength = 0;
	vkBufferImageCopy.bufferImageHeight = 0;
	vkBufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	vkBufferImageCopy.imageSubresource.mipLevel = 0;
	vkBufferImageCopy.imageSubresource.baseArrayLayer = 0;
	vkBufferImageCopy.imageSubresource.layerCount = 1;

	vkBufferImageCopy.imageOffset = { 0, 0, 0 };
	vkBufferImageCopy.imageExtent = {
		m_width,
		m_height,
		1	// not 3D
	};

	CRM(pVKCommandBuffers->CopyBufferToImage(0, m_vkStagingBuffer, m_pVKImage->GetVKImageHandle(), vkBufferImageCopy),
		"Failed to copy staging buffer to image");

	CRM(pVKCommandBuffers->End(0), "Failed to start command buffer");

Error:
	return r;
}

RESULT VKTexture::Initialize() {
	RESULT r = R::OK;

	CRM(LoadImage(), "Failed to load image");
	CNM(m_pImage, "Image failed to load");

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

	m_pVKImage = VKImage::make(
		m_vkPhysicalDevice, 
		m_vkLogicalDevice, 
		m_width, m_height, 
		VK_FORMAT_R8G8B8A8_SRGB, 
		VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);
	CNM(m_pVKImage, "Failed to create VKImage");

	// Transition the image layout
	CRM(m_pVKImage->TranisitionImageLayout(m_pVKCommandPool,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL), "Failed to transition image layout");
	
	// Copy staging buffer to image
	CRM(CopyStagingBufferToImage(), "Failed to copy staging buffer to image");

Error:
	return r;
}

RESULT VKTexture::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);

	m_pVKImage = nullptr;

	vkDestroyBuffer(m_vkLogicalDevice, m_vkStagingBuffer, nullptr);
	vkFreeMemory(m_vkLogicalDevice, m_vkStagingBufferDeviceMemory, nullptr);

Error:
	return r;
}

EPRef<VKTexture> VKTexture::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKCommandPool>& pVKCommandPool,
	const EPString<char>& strFilename
) {
	RESULT r = R::OK;
	EPRef<VKTexture> pVKTexture = nullptr;

	pVKTexture = new VKTexture(vkPhysicalDevice, vkLogicalDevice, pVKCommandPool, strFilename);
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
	const EPRef<VKCommandPool>& pVKCommandPool,
	const EPString<char>& strFilename
) :
	texture(strFilename),
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKCommandPool(pVKCommandPool)
{
	//
}

VKTexture::~VKTexture() {
	Kill();
}