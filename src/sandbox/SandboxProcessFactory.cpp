#include "SandboxProcessFactory.h"

#ifdef EPOCH_SANDBOX_WIN32
	#include "sandbox/win64/Win64SandboxWindowProcess.h"
	#include "sandbox/win64/Win64SandboxConsoleProcess.h"


	EPRef<SandboxProcess> SandboxProcessFactory::InternalMakeWin64(const EPString<char>& strName, SandboxProcess::type procType) {
		RESULT r = R::OK;

		EPRef<SandboxProcess> pSandboxProcess = nullptr;

		switch (procType) {
			case SandboxProcess::type::window: {
				pSandboxProcess = new Win64SandboxWindowProcess();
				CNM(pSandboxProcess, "Failed to allocate Win64SandboxWindowProcess");

				// Initialize is handled in the process itself
				//CRM(pSandboxProcess->Initialize(), "Failed to initialize Win64SandboxWindowProcess");

				CRM(pSandboxProcess->SetName(strName), "Failed to set name of Win64SandboxWindowProcess");

			} break;

			case SandboxProcess::type::console: {
				pSandboxProcess = new Win64SandboxConsoleProcess();
				CNM(pSandboxProcess, "Failed to allocate Win64SandboxConsoleProcess");

				// Initialize is handled in the process itself
				//CRM(pSandboxProcess->Initialize(), "Failed to initialize Win64SandboxConsoleProcess");

				CRM(pSandboxProcess->SetName(strName), "Failed to set name of Win64SandboxConsoleProcess");
			} break;
		}

	Error:
		return pSandboxProcess;
	}
#endif

EPRef<SandboxProcess> SandboxProcessFactory::InternalMake(UNUSED const EPString<char>& strName, 
	UNUSED SandboxProcess::type procType, 
	sandbox::platform plat) 
{
	RESULT r = R::OK;

	switch (plat) {
		#ifdef EPOCH_SANDBOX_WIN32
			case sandbox::platform::win64: {
				return SandboxProcessFactory::InternalMakeWin64(strName, procType);
			} break;
		#endif

		default: {
			CRM(R::NOT_IMPLEMENTED,
				"Platform %s not yet supported",
					sandbox::GetPlatformName(plat));
		} break;
	}



Error:
	return nullptr;
}