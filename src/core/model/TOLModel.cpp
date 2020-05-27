#include "TOLModel.h"

RESULT TOLModel::Initialize() {
	RESULT r = R::OK;

	// load model 


Error:
	return r;
}

RESULT TOLModel::Kill() {
	RESULT r = R::OK;

	//

Error:
	return r;
}

EPRef<TOLModel> TOLModel::InternalMake(const EPString<char>& strModelFilename, const EPString<char>& strTextureFilename) {
	RESULT r = R::OK;
	EPRef<TOLModel> pTOLModel = nullptr;

	pTOLModel = new TOLModel(strModelFilename, strTextureFilename);
	CNM(pTOLModel, "Failed to allocate TOL model");

	CRM(pTOLModel->Initialize(), "Failed to initialize tol model");

Success:
	return pTOLModel;

Error:
	pTOLModel = nullptr;
	return nullptr;
}