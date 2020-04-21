#include "SandboxProcessFactory.h"

#include "sandbox/win64/Win64SandboxProcess.h"

EPRef<SandboxProcess> SandboxProcessFactory::InternalMakeWin64(const EPString<char>& strName, SandboxProcess::type procType) {
	RESULT r = R::OK;

	EPRef<SandboxProcess> pSandboxProcess = nullptr;


	switch (procType) {
		case SandboxProcess::type::window: {
			pSandboxProcess = new Win64SandboxProcess();
			CNM(pSandboxProcess, "Failed to allocate Win64SandboxProcess");

			// Initialize is handled in the process itself
			//CRM(pSandboxProcess->Initialize(), "Failed to initialize Win64SandboxProcess");

			CRM(pSandboxProcess->SetName(strName), "Failed to set name of Win64SandboxProcess");

		} break;

		case SandboxProcess::type::console: {

			CRM(R::NOT_IMPLEMENTED, 
				"Console not supported for %s", 
					sandbox::GetPlatformName(sandbox::platform::win64));

		} break;
	}

Error:
	return pSandboxProcess;
}

EPRef<SandboxProcess> SandboxProcessFactory::InternalMake(const EPString<char>& strName, 
	SandboxProcess::type procType, 
	sandbox::platform plat) 
{
	RESULT r = R::OK;

	switch (plat) {
		case sandbox::platform::win64: {
			return SandboxProcessFactory::InternalMakeWin64(strName, procType);
		} break;

		default: {
			CRM(R::NOT_IMPLEMENTED,
				"Platform %s not yet supported",
					sandbox::GetPlatformName(plat));
		} break;
	}



Error:
	return nullptr;
}