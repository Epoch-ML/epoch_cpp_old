#include "VulkanHAL.h"

#include "core/types/EPRef.h"
#include "sandbox/SandboxWindowProcess.h"
#include "sandbox/win64/Win64SandboxWindowProcess.h"

#include "VulkanUtilities.h"

#include <string>

VulkanHAL::VulkanHAL() {
	//
}

VulkanHAL::~VulkanHAL() {
	//
}

RESULT VulkanHAL::EnumerateInstanceExtensions() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;

	int i = 0;
	int count = 0;

	CVKRM(vkEnumerateInstanceExtensionProperties(nullptr, &m_vkEnumeratedExtensionCount, nullptr), 
		"vkEnumerateInstanceExtensionProperties failed: %s", VkErrorString(vkr));
	CBM(m_vkEnumeratedExtensionCount != 0, "vkEnumerateInstanceExtensionProperties resulted in zero extensions");

	m_vkEnumeratedExtensions = EPVector<VkExtensionProperties>(m_vkEnumeratedExtensionCount);
	CVKRM(vkEnumerateInstanceExtensionProperties(nullptr, &m_vkEnumeratedExtensionCount, m_vkEnumeratedExtensions.data(m_vkEnumeratedExtensionCount)), 
		"vkEnumerateInstanceExtensionProperties failed: %s", VkErrorString(vkr));
	CBM(m_vkEnumeratedExtensionCount != 0, "vkEnumerateInstanceExtensionProperties resulted in zero extensions");

	for (auto& extension : m_vkEnumeratedExtensions) {
		DEBUG_LINEOUT("%d: v%d %s", ++i, extension.specVersion, extension.extensionName);
	}

	for (auto& reqExtension : m_RequiredExtensions) {
		bool fFound = false;
		for (auto& extension : m_vkEnumeratedExtensions) {
			if (strcmp(reqExtension.get<1, const char*>(), extension.extensionName) == 0) {
				m_vkExtensions.PushFront(extension);

				m_vkExtensionNames[count++] = extension.extensionName;
				fFound = true;
				break;
			}
		}

		CBM(fFound, "Failed to add required extension %s", reqExtension.get<1, const char*>());
	}

Error:
	return r;
}

RESULT VulkanHAL::EnumerateValidationLayers() {
	RESULT r = R::OK;

	if (m_fEnableValidationLayers == false)
		return R::OK;

	CVKRM(vkEnumerateInstanceLayerProperties(&m_vkValidationLayerCount, nullptr), 
		"vkEnumerateInstanceLayerProperties failed");

	m_vkAvailableValidationLayers = EPVector<VkLayerProperties>(m_vkValidationLayerCount);
	CVKRM(vkEnumerateInstanceLayerProperties(&m_vkValidationLayerCount, m_vkAvailableValidationLayers.data()),
		"vkEnumerateInstanceLayerProperties failed");

	// Ensure all layers are there
	for (auto& szLayerName : m_vkRequiredValidationLayers) {
		bool fFound = false;

		for (const auto& layerProperties : m_vkAvailableValidationLayers) {
			if (strcmp(szLayerName, layerProperties.layerName) == 0) {
				fFound = true;
				break;
			}
		}

		CBM(fFound, "Failed to find validation layer %s", szLayerName);
	}

Error:
	return r;
}

RESULT VulkanHAL::Initialize() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;

	DEBUG_LINEOUT("Initializing Vulkan");

	EPRef<SandboxWindowProcess> pSBWindowProcess = GetSBWindowProcess();
	CNM(pSBWindowProcess, "Initialization needs a valid sandbox window process");

	CRM(EnumerateInstanceExtensions(), "Failed to enumerate VK extensions");
	CRM(EnumerateValidationLayers(), "Failed to Enumerate Validation Layers");

	DEBUG_LINEOUT("Extensions Supported: %d", m_vkEnumeratedExtensionCount);

	// VkApplicationInfo 

	m_vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_vkApplicationInfo.pApplicationName = pSBWindowProcess->GetName().c_str();
	m_vkApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	m_vkApplicationInfo.pEngineName = "No Engine";
	m_vkApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	m_vkApplicationInfo.apiVersion = VK_API_VERSION_1_0;

	// VkInstanceCreateInfo 

	m_vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	m_vkInstanceCreateInfo.pApplicationInfo = &m_vkApplicationInfo;
	
	// Instance Extensions
	m_vkInstanceCreateInfo.enabledExtensionCount = (uint32_t)m_vkExtensions.size();
	m_vkInstanceCreateInfo.ppEnabledExtensionNames = m_vkExtensionNames;
	
	// Enabled Layers
	m_vkInstanceCreateInfo.enabledLayerCount = 0;
	m_vkInstanceCreateInfo.ppEnabledLayerNames = nullptr;

	CVKRM(vkCreateInstance(&m_vkInstanceCreateInfo, nullptr, &m_vkInstance), "vkCreateInstance failed: %s", VkErrorString(vkr));
	CNM(m_vkInstance, "Vulkan Instance not created properly");

Error:
	return r;
}

RESULT VulkanHAL::Kill() {
	RESULT r = R::OK;

	vkDestroyInstance(m_vkInstance, nullptr);


Error:
	return r;
}