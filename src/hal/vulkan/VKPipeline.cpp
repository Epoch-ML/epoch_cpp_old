#include "VKPipeline.h"

RESULT VKPipeline::Initialize() {
	RESULT r = R::OK;

	CNM(m_vkLogicalDevice, "Cannot create pipeline without a valid logical device");

	m_pVertexShader = VKShader::make("vert.spv", m_vkLogicalDevice);
	CNM(m_pVertexShader, "Failed to create vertex shader");

	m_pFragmentShader = VKShader::make("frag.spv", m_vkLogicalDevice);
	CNM(m_pFragmentShader, "Failed to create fragment shader");

Error:
	return r;
}

RESULT VKPipeline::Kill() {
	RESULT r = R::OK;

	m_pVertexShader = nullptr;
	m_pFragmentShader = nullptr;

Error:
	return r;
}

EPRef<VKPipeline> VKPipeline::InternalMake(VkDevice vkLogicalDevice) {
	RESULT r = R::OK;
	EPRef<VKPipeline> pVKPipeline = nullptr;

	pVKPipeline = new VKPipeline(vkLogicalDevice);
	CNM(pVKPipeline, "Failed to allocate pipeline");

	CRM(pVKPipeline->Initialize(), "Failed to initialize VK pipeline");

Success:
	return pVKPipeline;

Error:
	pVKPipeline = nullptr;
	return nullptr;
}