#include "HALFactory.h"

#include "hal/vulkan/VulkanHAL.h"

EPRef<HAL> HALFactory::InternalMakeVulkan(EPRef<SandboxProcess> pSBProcess) {
	RESULT r = R::OK;

	EPRef<HAL> pHAL = nullptr;

	pHAL = new VulkanHAL();
	CNM(pHAL, "Failed to allocate VulkanHAL");

	// Initialize is handled in the process itself
	CRM(pHAL->Initialize(pSBProcess), "Failed to initialize Win64SandboxWindowProcess");

Error:
	return pHAL;
}

EPRef<HAL> HALFactory::InternalMake(HAL::type halType, EPRef<SandboxProcess> pSBProcess) {
	RESULT r = R::OK;

	switch (halType) {
	case HAL::type::vulkan: {
			return HALFactory::InternalMakeVulkan(pSBProcess);
		} break;

		default: {
			CRM(R::NOT_IMPLEMENTED,
				"HAL %s not yet supported",
				HAL::GetTypeName(halType));
		} break;
	}

Error:
	return nullptr;
}