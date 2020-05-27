#ifndef VK_MODEL_H_
#define VK_MODEL_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Model
// epoch/src/hal/vulkan/VKModel.h

#include "core/model/model.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VulkanUtilities.h"

class VKCommandPool;

//template <typename TValue, int dimension> class VKVertex;
#include "VKVertex.h"

class VKModel :
	public model,
	virtual public EPFactoryMethod<VKModel, 
		VkPhysicalDevice, 
		VkDevice, 
		EPRef<VKCommandPool>, 
		const EPString<char>&
	>
{
protected:
	VKModel(
		VkPhysicalDevice vkPhysicalDevice,
		VkDevice vkLogicalDevice,
		EPRef<VKCommandPool> pVKCommandPool,
		const EPString<char>&);

public:
	virtual ~VKModel() override;

	// This seems like more of an object kind of thing
	RESULT Bind(VkCommandBuffer vkCommandBuffer);
	RESULT Draw(VkCommandBuffer vkCommandBuffer);

	static EPRef<VKModel> InternalMake(VkPhysicalDevice, VkDevice, EPRef<VKCommandPool>, const EPString<char>&);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	EPRef<VKCommandPool> m_pVKCommandPool = nullptr;

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

#endif // ! VK_MODEL_H_