#include "sandbox.h"

#include "SandboxProcessFactory.h"

const char* sandbox::kPlatformNames[] = {
	"win64",
	"osx",
	"linux",
	"android",
	"ios",
	"unix",
	"posix",
	"none"
};

sandbox::sandbox() {
	// empty
}

sandbox::~sandbox() {
	// empty
}

sandbox::platform sandbox::GetCurrentPlatform() {
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)	
		return sandbox::platform::win64;
	#elif __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR
		return sandbox::platform::ios;
	#elif TARGET_OS_IPHONE
		return sandbox::platform::ios;
	#elif TARGET_OS_MAC
		return sandbox::platform::osx;
	#else
	#	error "Unknown Apple platform"
	#endif
	#elif __ANDROID__
		return sandbox::platform::android;
	#elif __linux__
		return sandbox::platform::linux;
	#elif __unix__	// all unix systems not caught above
		return sandbox::platform::unix;
	#elif defined(_POSIX_VERSION)
		return sandbox::platform::posix;
	#else
	#   error "Unknown sandbox platform"
	#endif
}

EPRef<sandbox> sandbox::make() {
	RESULT r = R::OK;

	EPRef<sandbox> pSandbox = new sandbox();
	CNM(pSandbox, "Failed to create sandbox");

	CRM(pSandbox->SetPlatform(sandbox::GetCurrentPlatform()), "Failed to set Sandbox platform");


Success:
	return pSandbox;

Error:
	return nullptr;
}

RESULT sandbox::CreateSandboxProcess(EPString<char> strProcessName, SandboxProcess::type procType) {
	RESULT r = R::OK;

	EPRef<SandboxProcess> pSandboxProcess = SandboxProcessFactory::make(strProcessName, procType, m_platform);
	CNM(pSandboxProcess, "Failed to create procType: %d process:%s on platform: %s",
		procType, strProcessName.c_str(), GetPlatformName(m_platform));

Error:
	return r;
}

EPRef<SandboxProcess> sandbox::GetSandboxProcess(EPString<char> strProcessName) {
	EPRef<SandboxProcess> pSandboxProcess = nullptr;

	auto it = m_sandboxProcesses.begin();

	for(auto &pProcess : m_sandboxProcesses) {
		if (pProcess->GetName() == strProcessName) {
			pSandboxProcess = pProcess;
			break;
		}
	}

	return pSandboxProcess;
}

RESULT sandbox::RunSandboxProcess(EPString<char> strProcessName) {
	RESULT r = R::OK;

	auto pSandboxProcess = GetSandboxProcess(strProcessName);
	CNM(pSandboxProcess, "%s sandbox process not found", strProcessName.c_str());

	CRM(pSandboxProcess->Run(), "Failed to run Sandbox Process");

Error:
	return r;
}

RESULT sandbox::KillSandboxProcess(EPString<char> strProcessName) {
	RESULT r = R::OK;

	auto pSandboxProcess = GetSandboxProcess(strProcessName);
	CNM(pSandboxProcess, "%s sandbox process not found", strProcessName.c_str());

	CRM(pSandboxProcess->Kill(), "Failed to run Sandbox Process");

Error:
	return r;
}