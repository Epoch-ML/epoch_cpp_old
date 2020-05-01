#include "VKPipeline.h"

RESULT VKPipeline::Initialize() {
	RESULT r = R::OK;

	m_pVertexShader = VKShader::make("vert.spv");
	CNM(m_pVertexShader, "Failed to create vertex shader");

	m_pFragmentShader = VKShader::make("frag.spv");
	CNM(m_pFragmentShader, "Failed to create fragment shader");

Error:
	return r;
}

RESULT VKPipeline::Kill() {
	RESULT r = R::OK;

	//

Error:
	return r;
}

EPRef<VKPipeline> VKPipeline::InternalMake() {
	RESULT r = R::OK;
	EPRef<VKPipeline> pVKPipeline = nullptr;

	pVKPipeline = new VKPipeline();
	CNM(pVKPipeline, "Failed to allocate pipeline");

	CRM(pVKPipeline->Initialize(), "Failed to initialize VK pipeline");

Success:
	return pVKPipeline;

Error:
	pVKPipeline = nullptr;
	return nullptr;
}