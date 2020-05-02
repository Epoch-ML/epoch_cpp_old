#ifndef VULKAN_PIPELINE_H_
#define VULKAN_PIPELINE_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Pipeline
// epoch/src/hal/vulkan/VKPipeline.h

#include "hal/pipeline.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"

#include "VKShader.h"
#include "VKSwapchain.h"

class VKPipeline :
	public pipeline,
	public EPFactoryMethod<VKPipeline, VkDevice, const EPRef<VKSwapchain>&>
{
private:
	VKPipeline(VkDevice vkLogicalDevice, const EPRef<VKSwapchain>& pVKSwapchain)  :
		m_vkLogicalDevice(vkLogicalDevice),
		m_pVKSwapchain(pVKSwapchain)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKPipeline() override {
		Kill();
	}

	static EPRef<VKPipeline> InternalMake(VkDevice, const EPRef<VKSwapchain>&);

	const VkRenderPass GetVKRenderPassHandle() const { return m_vkRenderPass; }
	const VkPipeline GetVKPipelineHandle() const { return m_vkGraphicsPipeline; }

private:
	VkDevice m_vkLogicalDevice = nullptr;
	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;

	// TODO: Replace with system to register/create vk pipelines
	EPRef<VKShader> m_pVertexShader = nullptr;
	EPRef<VKShader> m_pFragmentShader = nullptr;

	// Fixed pipeline stages
	// TODO: Move into objects
	VkPipelineVertexInputStateCreateInfo m_vkPipelineVertexInputStateCreateInfo = {};
	VkPipelineInputAssemblyStateCreateInfo m_vkPipelineInputAssemblyStateCreateInfo = {};
	VkPipelineViewportStateCreateInfo m_vkPipelineViewportStateCreateInfo = {};
	VkPipelineRasterizationStateCreateInfo m_vkPipelineRasterizationStateCreateInfo = {};
	VkPipelineMultisampleStateCreateInfo m_vkPipelineMultisampleStateCreateInfo = {};
	VkPipelineColorBlendStateCreateInfo m_vkPipelineColorBlendStateCreateInfo = {};

	VkPipelineColorBlendAttachmentState m_vkPipelineColorBlendAttachmentState = {};
	VkViewport m_vkViewport = {};
	VkRect2D m_vkrect2DScissor = {};

	bool m_fBlendEnabled = false;

	VkPipelineDynamicStateCreateInfo m_vkPipelineDynamicStateCreateInfo = {};
	EPVector<VkDynamicState> m_vkDynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineLayoutCreateInfo m_vkPipelineLayoutCreateInfo = {};
	VkPipelineLayout m_vkPipelineLayout = nullptr;

	// TODO: Render pass is here, move into an object
	VkAttachmentDescription m_vkAttachmentDescription = {};
	VkAttachmentReference m_vkAttachmentReference = {};

	VkSubpassDescription m_vkSubpassDescription = {};

	// Render pass
	VkRenderPassCreateInfo m_vkRenderPassCreateInfo = {};
	VkRenderPass m_vkRenderPass = nullptr;

	VkGraphicsPipelineCreateInfo m_vkGraphicsPipelineCreateInfo = {};

	VkPipeline m_vkGraphicsPipeline = nullptr;

};

#endif // ! VULKAN_PIPELINE_H_