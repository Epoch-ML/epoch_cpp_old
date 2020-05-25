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

class VKCommandPool;

//template <typename TValue, int dimension> class VKVertex;
#include "VKVertex.h"

class VKVertexBuffer : 
	public buffer,
	virtual public EPFactoryMethod<VKVertexBuffer, VkPhysicalDevice, VkDevice, EPRef<VKCommandPool>, VkQueue>
{
protected:
	VKVertexBuffer(
		VkPhysicalDevice vkPhysicalDevice, 
		VkDevice vkLogicalDevice, 
		EPRef<VKCommandPool> pVKCommandPool, 
		VkQueue vkQueue);

public:
	virtual ~VKVertexBuffer() override;

	virtual RESULT Bind() override {
		return R::NOT_IMPLEMENTED;
	}

	// This seems like more of an object kind of thing
	RESULT Bind(VkCommandBuffer vkCommandBuffer);
	RESULT Draw(VkCommandBuffer vkCommandBuffer);
	RESULT DrawIndexed(VkCommandBuffer vkCommandBuffer);

	static EPRef<VKVertexBuffer> InternalMake(VkPhysicalDevice, VkDevice, EPRef<VKCommandPool>, VkQueue);

	// TODO: make it like... better
	RESULT InitializeAsTriangle() {
		RESULT r = R::OK;

		// Set up the command buffer data
		// TODO: This is temporary just for testing
		m_vertices = {
			VKVertex<float, 4>({ 0.0f, 0.0f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}),
			VKVertex<float, 4>({ 0.5f, 0.0f,  0.5f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}),
			VKVertex<float, 4>({-0.5f, 0.0f,  0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 1.0f})
		};

		m_indices = { 0, 1, 2 };

	Error:
		return r;
	}

	RESULT InitializeAsQuad() {
		RESULT r = R::OK;

		// Set up the command buffer data
		// TODO: This is temporary just for testing
		m_vertices = {
			VKVertex<float, 4>({-0.5f, 0.0f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}),
			VKVertex<float, 4>({ 0.5f, 0.0f, -0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}),
			VKVertex<float, 4>({ 0.5f, 0.0f,  0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}),
			VKVertex<float, 4>({-0.5f, 0.0f,  0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f})
		};

		m_indices = {
			0, 1, 2,
			2, 3, 0
		};

	Error:
		return r;
	}

	RESULT InitializeAsCube() {
		RESULT r = R::OK;

		// Set up the command buffer data
		// TODO: This is temporary just for testing
		m_vertices = {
			// Top
			VKVertex<float, 4>({-0.5f, 0.5f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}),	// 0 top left back
			VKVertex<float, 4>({ 0.5f, 0.5f, -0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}),	// 1 top right back
			VKVertex<float, 4>({ 0.5f, 0.5f,  0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}),	// 2	 top right front
			VKVertex<float, 4>({-0.5f, 0.5f,  0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}),	// 3 top left front

			VKVertex<float, 4>({-0.5f, -0.5f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}),	// 4 bottom left back
			VKVertex<float, 4>({ 0.5f, -0.5f, -0.5f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}),	// 5 bottom right back
			VKVertex<float, 4>({ 0.5f, -0.5f,  0.5f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}),	// 6 bottom right front
			VKVertex<float, 4>({-0.5f, -0.5f,  0.5f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f})	// 7 bottom left front
		};

		m_indices = {
			// top
			1, 0, 3,
			3, 2, 1,
			// bottom
			5, 6, 7,
			7, 4, 5,
			// front
			2, 3, 7,
			7, 6, 2,
			// back
			0, 1, 5,
			5, 4, 0,
			// left
			3, 0, 4,
			4, 7, 3,
			// right
			1, 2, 6,
			6, 5, 1
		};

	Error:
		return r;
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	EPRef<VKCommandPool> m_pVKCommandPool = nullptr;
	VkQueue m_vkQueue = nullptr;

	// TODO: need a better way in the future
	EPVector<VKVertex<float, 4>> m_vertices;
	EPVector<uint16_t> m_indices;

// Staging Buffer
	VkBuffer m_vkStagingBuffer = nullptr;
	VkDeviceMemory m_vkStagingBufferDeviceMemory = nullptr;

// Vertex Buffer
	VkBuffer m_vkVertexBuffer = nullptr;
	VkDeviceMemory m_vkVertexBufferDeviceMemory = nullptr;

// Index Buffer
	VkBuffer m_vkIndexBuffer = nullptr;
	VkDeviceMemory m_vkIndexBufferDeviceMemory = nullptr;
};

#endif // ! VULKAN_FRAMEBUFFER_H_