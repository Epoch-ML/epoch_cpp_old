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

class VKCommandBuffers :
	public command_buffer,
	public EPFactoryMethod<VKCommandBuffers, const EPRef<VKCommandPool>&>
{
private:
	VKCommandBuffers(const EPRef<VKCommandPool>& pVKCommandPool);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKCommandBuffers() override;

	static EPRef<VKCommandBuffers> InternalMake(const EPRef<VKCommandPool>& pVKCommandPool);

	RESULT RecordCommandBuffers();
	const VkCommandBuffer* GetCommandBufferHandle(uint32_t index) const {
		return &(m_vkCommandBuffers[index]);
	}

private:
	EPRef<VKCommandPool> m_pVKCommandPool = nullptr;

	VkCommandBufferAllocateInfo m_vkCommandBufferAllocateInfo = {};
	
	EPVector<VkCommandBuffer> m_vkCommandBuffers;
};

#endif // ! VULKAN_COMMAND_BUFFER_H_