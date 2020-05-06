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

#include "VKBuffer.h"

class VKVertexBuffer : 
	public VKBuffer,
	virtual public EPFactoryMethod<VKVertexBuffer, VkPhysicalDevice, VkDevice, size_t>
{
protected:
	VKVertexBuffer(VkPhysicalDevice vkPhysicalDevice, VkDevice vkLogicalDevice, size_t size) :
		VKBuffer(vkPhysicalDevice, vkLogicalDevice, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
	{
		//
	}
public:
	RESULT Bind(VkCommandBuffer vkCommandBuffer);

	static EPRef<VKVertexBuffer> InternalMake(VkPhysicalDevice, VkDevice, size_t);

private:
	//
};

#endif // ! VULKAN_FRAMEBUFFER_H_