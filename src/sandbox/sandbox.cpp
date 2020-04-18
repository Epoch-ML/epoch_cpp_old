#include "sandbox.h"

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