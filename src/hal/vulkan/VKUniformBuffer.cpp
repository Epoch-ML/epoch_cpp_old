#include "VKUniformBuffer.h"

#include <chrono>

#include "VKCommandPool.h"

#include "VKVertex.h"
#include "VKBuffer.h"

#include "core/math/math.h"
#include "core/math/matrix/rotation.h"
#include "core/math/matrix/projection.h"
#include "core/math/matrix/view.h"

#include "core/math/vector.h"
#include "core/math/point.h"

RESULT VKUniformBuffer::Initialize() {
	RESULT r = R::OK;

	// Set up the buffers and device memory for the uniform buffer

	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	
	m_vkUniformBuffers = EPVector<VkBuffer>(m_pVKSwapchain->GetSwapchainImageCount(), true);
	m_vkUniformBufferDeviceMemories = EPVector<VkDeviceMemory>(m_pVKSwapchain->GetSwapchainImageCount(), true);
	
	for (size_t i = 0; i < m_pVKSwapchain->GetSwapchainImageCount(); i++) {

		CVKRM(VKBuffer::CreateBuffer(
			m_vkPhysicalDevice,
			m_vkLogicalDevice,
			bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_vkUniformBuffers[i],
			m_vkUniformBufferDeviceMemories[i]),
			"Failed to Create Uniform Buffer");
		
		CN(m_vkUniformBuffers[i]);
		CN(m_vkUniformBufferDeviceMemories[i]);
	}

Error:
	return r;
}

RESULT VKUniformBuffer::Update(uint32_t currentImage) {
	RESULT r = R::OK;

	point<> ptEye = point<>(2.0f, 2.0f, 2.0f);
	point<> ptOrigin = point<>();

	// TODO: get rid of this from here (for testing)
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	m_uniformBufferObject.m_mat4Model = rotation(math::axis::Y, 0.0f);
	m_uniformBufferObject.m_mat4View = view<>::MakeLookAtViewMatrix(ptEye, ptOrigin, vector<>::j());
	m_uniformBufferObject.m_mat4Projection = projection<float>(
		projection<float>::type::PERSPECTIVE,
		(float)m_pVKSwapchain->GetExtentsWidth(), (float)m_pVKSwapchain->GetExtentsHeight(),
		0.1f, 10.0f, 45.0f);

Error:
	return r;
}

RESULT VKUniformBuffer::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);

	for (size_t i = 0; i < m_pVKSwapchain->GetSwapchainImageCount(); i++) {
		//createBuffer(bufferSize, , , uniformBuffers[i], uniformBuffersMemory[i]);

		CN(m_vkUniformBuffers[i]);
		vkDestroyBuffer(m_vkLogicalDevice, m_vkUniformBuffers[i], nullptr);

		CN(m_vkUniformBufferDeviceMemories[i]);
		vkFreeMemory(m_vkLogicalDevice, m_vkUniformBufferDeviceMemories[i], nullptr);

		m_vkUniformBuffers[i] = nullptr;
		m_vkUniformBufferDeviceMemories[i] = nullptr;
	}

Error:
	return r;
}

EPRef<VKUniformBuffer> VKUniformBuffer::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKSwapchain>& pVKSwapchain
) {
	RESULT r = R::OK;
	EPRef<VKUniformBuffer> pVKUniformBuffer = nullptr;

	pVKUniformBuffer = new VKUniformBuffer(vkPhysicalDevice, vkLogicalDevice, pVKSwapchain);
	CNM(pVKUniformBuffer, "Failed to allocate vk vertex buffer");

	CRM(pVKUniformBuffer->Initialize(), "Failed to initialize VK vertex buffer");

Success:
	return pVKUniformBuffer;

Error:
	pVKUniformBuffer = nullptr;
	return nullptr;
}

VKUniformBuffer::VKUniformBuffer(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice,
	const EPRef<VKSwapchain> &pVKSwapchain
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice),
	m_pVKSwapchain(pVKSwapchain)
{
	//
}

VKUniformBuffer::~VKUniformBuffer() {
	Kill();
}