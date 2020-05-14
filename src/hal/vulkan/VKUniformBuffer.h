#ifndef VULKAN_UNIFORM_BUFFER_H_
#define VULKAN_UNIFORM_BUFFER_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Uniform Buffer
// epoch/src/hal/vulkan/VKUniformBuffer.h

#include "hal/buffer.h"

#include <vulkan/vulkan.h>

#include "core/math/matrix/transform.h"

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

#include "VulkanUtilities.h"

class VKCommandPool;
class VKSwapchain;

class VKUniformBuffer :
	public buffer,
	virtual public EPFactoryMethod<VKUniformBuffer, VkPhysicalDevice, VkDevice, const EPRef<VKSwapchain>&>
{
public:

// TODO: This is temporary
#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
	struct UniformBufferObject {
		alignas(16) transform<> m_mat4Model;
		alignas(16) transform<> m_mat4View;
		alignas(16) transform<> m_mat4Projection;
	} m_uniformBufferObject;
#pragma pack(pop)

protected:
	VKUniformBuffer(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice, const EPRef<VKSwapchain> &pVKSwapchain);

public:
	virtual ~VKUniformBuffer() override;

	virtual RESULT Bind() override {
		return R::NOT_IMPLEMENTED;
	}

	static EPRef<VKUniformBuffer> InternalMake(VkPhysicalDevice, VkDevice, const EPRef<VKSwapchain>&);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

	virtual RESULT Update(uint32_t currentImage) override;

	VkBuffer GetUniformBuffer(uint32_t index) {
		return m_vkUniformBuffers[index];
	}

	// TODO: this is dumb as shit
	size_t GetUniforBufferObjectSize() {
		return sizeof(UniformBufferObject);
	}

private:
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;
	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;

	EPVector<VkBuffer> m_vkUniformBuffers;
	EPVector<VkDeviceMemory> m_vkUniformBufferDeviceMemories;
};

#endif // ! VULKAN_UNIFORM_BUFFER_H_