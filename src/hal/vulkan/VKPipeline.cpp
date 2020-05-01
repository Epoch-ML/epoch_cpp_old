#include "VKPipeline.h"

RESULT VKPipeline::Initialize() {
	RESULT r = R::OK;

	//

Error:
	return r;
}

RESULT VKPipeline::Kill() {
	RESULT r = R::OK;

	//

Error:
	return r;
}

EPRef<VKPipeline> VKPipeline::make() {
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