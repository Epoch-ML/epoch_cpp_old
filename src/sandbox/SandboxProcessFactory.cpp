#include "SandboxProcessFactory.h"

EPRef<SandboxProcess> SandboxProcessFactory::InternalMake(EPString<char>, SandboxProcess::type, sandbox::platform) {
	RESULT r = R::OK;
	
	EPRef<SandboxProcess> pSandboxProcess = nullptr;

	// TODO: 

Error:
	return pSandboxProcess;
}