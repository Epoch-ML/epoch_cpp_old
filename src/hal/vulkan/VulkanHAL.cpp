#include "VulkanHAL.h"

#include <vulkan/vulkan.h>

VulkanHAL::VulkanHAL() {
	//
}

VulkanHAL::~VulkanHAL() {
	//
}

RESULT VulkanHAL::Initialize(const EPRef<SandboxProcess>& pSBProcess) {
	RESULT r = R::OK;

	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	DEBUG_LINEOUT("Extensions Supported: %d", extensionCount);

Error:
	return r;
}