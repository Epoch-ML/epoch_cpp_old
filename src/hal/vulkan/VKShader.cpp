#include "VKShader.h"

RESULT VKShader::Initialize() {
	RESULT r = R::OK;

	//

Error:
	return r;
}

RESULT VKShader::Kill() {
	RESULT r = R::OK;

	//

Error:
	return r;
}

EPRef<VKShader> VKShader::make() {
	RESULT r = R::OK;
	EPRef<VKShader> pVKShader = nullptr;

	pVKShader = new VKShader();
	CNM(pVKShader, "Failed to allocate shader");

	CRM(pVKShader->Initialize(), "Failed to initialize VK shader");

Success:
	return pVKShader;

Error:
	pVKShader = nullptr;
	return nullptr;
}