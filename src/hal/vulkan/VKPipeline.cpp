#include "VKPipeline.h"

#include "VulkanUtilities.h"

RESULT VKPipeline::Initialize() {
	RESULT r = R::OK;

	CNM(m_vkLogicalDevice, "Cannot create pipeline without a valid logical device");

	m_pVertexShader = VKShader::make(m_vkLogicalDevice, "vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
	CNM(m_pVertexShader, "Failed to create vertex shader");

	m_pFragmentShader = VKShader::make(m_vkLogicalDevice, "frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
	CNM(m_pFragmentShader, "Failed to create fragment shader");

	// Fixed stages
	// TODO: This should all be moved into objects
	
	// Vertex Input Stage
	m_vkPipelineVertexInputStateCreateInfo .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_vkPipelineVertexInputStateCreateInfo .vertexBindingDescriptionCount = 0;
	m_vkPipelineVertexInputStateCreateInfo .pVertexBindingDescriptions = nullptr; // Optional
	m_vkPipelineVertexInputStateCreateInfo .vertexAttributeDescriptionCount = 0;
	m_vkPipelineVertexInputStateCreateInfo .pVertexAttributeDescriptions = nullptr; // Optional

	// Input assembly
	m_vkPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_vkPipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	m_vkPipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	// Viewport
	m_vkViewport.x = 0.0f;
	m_vkViewport.y = 0.0f;
	m_vkViewport.width = (float)m_pVKSwapchain->GetExtentsWidth();
	m_vkViewport.height = (float)m_pVKSwapchain->GetExtentsHeight();
	m_vkViewport.minDepth = 0.0f;
	m_vkViewport.maxDepth = 1.0f;

	// viewport Scissor
	m_vkrect2DScissor.offset = { 0, 0 };
	m_vkrect2DScissor.extent = { m_pVKSwapchain->GetExtentsWidth(), m_pVKSwapchain->GetExtentsHeight() };

	// Viewport state 
	m_vkPipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_vkPipelineViewportStateCreateInfo.viewportCount = 1;
	m_vkPipelineViewportStateCreateInfo.pViewports = &m_vkViewport;
	m_vkPipelineViewportStateCreateInfo.scissorCount = 1;
	m_vkPipelineViewportStateCreateInfo.pScissors = &m_vkrect2DScissor;

	// Rasterizer
	m_vkPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	m_vkPipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	m_vkPipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	m_vkPipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	m_vkPipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
	m_vkPipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	m_vkPipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	m_vkPipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	m_vkPipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
	m_vkPipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f; // Optional
	m_vkPipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f; // Optional

	// Multisampling
	m_vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_vkPipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	m_vkPipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	m_vkPipelineMultisampleStateCreateInfo.minSampleShading = 1.0f; // Optional
	m_vkPipelineMultisampleStateCreateInfo.pSampleMask = nullptr; // Optional
	m_vkPipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
	m_vkPipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE; // Optional

	// Color blending

	//fBlendEnabled
	//	finalColor.rgb = (srcColorBlendFactor * newColor.rgb) < colorBlendOp > (dstColorBlendFactor * oldColor.rgb);
	//	finalColor.a = (srcAlphaBlendFactor * newColor.a) < alphaBlendOp > (dstAlphaBlendFactor * oldColor.a);
	//else
	//	finalColor = newColor;
	//finalColor = finalColor & colorWriteMask;

	m_vkPipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	m_vkPipelineColorBlendAttachmentState.blendEnable = m_fBlendEnabled;
	m_vkPipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	m_vkPipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	m_vkPipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	m_vkPipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	m_vkPipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	m_vkPipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	// Alternative - TODO: move into an object
	//finalColor.rgb = newAlpha * newColor + (1 - newAlpha) * oldColor;
	//finalColor.a = newAlpha.a;
	//
	//m_vkPipelineColorBlendAttachmentState.blendEnable = VK_TRUE;
	//m_vkPipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	//m_vkPipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	//m_vkPipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	//m_vkPipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	//m_vkPipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	//m_vkPipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

	m_vkPipelineColorBendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	m_vkPipelineColorBendStateCreateInfo.logicOpEnable = VK_FALSE;
	m_vkPipelineColorBendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
	m_vkPipelineColorBendStateCreateInfo.attachmentCount = 1;
	m_vkPipelineColorBendStateCreateInfo.pAttachments = &m_vkPipelineColorBlendAttachmentState;
	m_vkPipelineColorBendStateCreateInfo.blendConstants[0] = 0.0f; // Optional
	m_vkPipelineColorBendStateCreateInfo.blendConstants[1] = 0.0f; // Optional
	m_vkPipelineColorBendStateCreateInfo.blendConstants[2] = 0.0f; // Optional
	m_vkPipelineColorBendStateCreateInfo.blendConstants[3] = 0.0f; // Optional

	// Dynamic States
	m_vkPipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	m_vkPipelineDynamicStateCreateInfo.dynamicStateCount = 2;
	m_vkPipelineDynamicStateCreateInfo.pDynamicStates = m_vkDynamicStates.data();

	// Create the pipeline layout
	m_vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	m_vkPipelineLayoutCreateInfo.setLayoutCount = 0; // Optional
	m_vkPipelineLayoutCreateInfo.pSetLayouts = nullptr; // Optional
	m_vkPipelineLayoutCreateInfo.pushConstantRangeCount = 0; // Optional
	m_vkPipelineLayoutCreateInfo.pPushConstantRanges = nullptr; // Optional

	CVKRM(vkCreatePipelineLayout(m_vkLogicalDevice, &m_vkPipelineLayoutCreateInfo, nullptr, &m_vkPipelineLayout),
		"Failed to creat the pipeline layout");
	CNM(m_vkPipelineLayout, "Failed to create pipeline layout");

Error:
	return r;
}

RESULT VKPipeline::Kill() {
	RESULT r = R::OK;

	m_pVertexShader = nullptr;
	m_pFragmentShader = nullptr;

	CN(m_vkLogicalDevice);
	CN(m_vkPipelineLayout);

	vkDestroyPipelineLayout(m_vkLogicalDevice, m_vkPipelineLayout, nullptr);

Error:
	return r;
}

EPRef<VKPipeline> VKPipeline::InternalMake(VkDevice vkLogicalDevice, const EPRef<VKSwapchain>& pVKSwapchain) {
	RESULT r = R::OK;
	EPRef<VKPipeline> pVKPipeline = nullptr;

	pVKPipeline = new VKPipeline(vkLogicalDevice, pVKSwapchain);
	CNM(pVKPipeline, "Failed to allocate pipeline");

	CRM(pVKPipeline->Initialize(), "Failed to initialize VK pipeline");

Success:
	return pVKPipeline;

Error:
	pVKPipeline = nullptr;
	return nullptr;
}