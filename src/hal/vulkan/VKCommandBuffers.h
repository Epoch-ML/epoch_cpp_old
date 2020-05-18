#ifndef VULKAN_COMMAND_BUFFER_H_
#define VULKAN_COMMAND_BUFFER_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Framebuffer
// epoch/src/hal/vulkan/VKFramebuffer.h

#include "hal/command_buffer.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

class VKCommandPool;
class VKBuffer;
class VKVertexBuffer;
class VKDescriptorSet;

class VKCommandBuffers :
	public command_buffer,
	//public EPFactoryMethod<VKCommandBuffers, const EPRef<VKCommandPool>&>,
	public EPFactoryMethod<VKCommandBuffers, 
		const EPRef<VKCommandPool>&,
		const EPRef<VKVertexBuffer>&,
		const EPRef<VKDescriptorSet>&
	>
{

public:
	VKCommandBuffers(const EPRef<VKCommandPool>&);
private:
	VKCommandBuffers(const EPRef<VKCommandPool>&, const EPRef<VKVertexBuffer>&, const EPRef<VKDescriptorSet>&);


	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	// TODO: This allows an external object to do this
	// might want to convert into the only way
	RESULT ProtoInitialize(uint32_t numBuffers = 1);
	RESULT Begin(uint32_t index = 0);
	RESULT End(uint32_t index = 0);
	
	RESULT Submit(VkQueue vkQueue);
	RESULT Submit(VkQueue vkQueue, uint32_t index);

public:
	virtual ~VKCommandBuffers() override;

	static EPRef<VKCommandBuffers> InternalMake(
		const EPRef<VKCommandPool>&, 
		const EPRef<VKVertexBuffer>&, 
		const EPRef<VKDescriptorSet>&
	);

	//static EPRef<VKCommandBuffers> InternalMake(const EPRef<VKCommandPool>&);

	RESULT RecordCommandBuffers();
	const VkCommandBuffer* GetCommandBufferHandle(uint32_t index) const {
		return &(m_vkCommandBuffers[index]);
	}

private:
	EPRef<VKCommandPool> m_pVKCommandPool = nullptr;

	VkCommandBufferAllocateInfo m_vkCommandBufferAllocateInfo = {};
	
	EPVector<VkCommandBuffer> m_vkCommandBuffers;

	// TODO: temp
	EPRef<VKDescriptorSet> m_pVKDescriptorSet = nullptr;
	EPRef<VKVertexBuffer> m_pVKVertexBuffer = nullptr;

	VKQueueFamilies m_vkQueueFamilies = {};
};

#endif // ! VULKAN_COMMAND_BUFFER_H_