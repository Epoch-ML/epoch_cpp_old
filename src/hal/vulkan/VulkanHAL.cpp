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
	int count = 0;

	if (m_fEnableValidationLayers == false)
		return R::SKIPPED;

	CVKRM(vkEnumerateInstanceLayerProperties(&m_vkValidationLayerCount, nullptr), 
		"vkEnumerateInstanceLayerProperties failed");

	m_vkAvailableValidationLayers = EPVector<VkLayerProperties>(m_vkValidationLayerCount);
	CVKRM(vkEnumerateInstanceLayerProperties(&m_vkValidationLayerCount, m_vkAvailableValidationLayers.data(m_vkValidationLayerCount)),
		"vkEnumerateInstanceLayerProperties failed");

	// Ensure all layers are there
	for (auto& szLayerName : m_vkRequiredValidationLayers) {
		bool fFound = false;

		for (const auto& layerProperties : m_vkAvailableValidationLayers) {
			if (strcmp(szLayerName, layerProperties.layerName) == 0) {
				m_vkValidationLayers.PushFront(layerProperties);
				m_vkValidationLayerNames[count++] = layerProperties.layerName;
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

	CRM(InitializeInstance(), "Failed to initialize VK instance");
	CRM(InitializeDebugMessenger(true), "Failed to initialize Debug Messenger");

Error:
	return r;
}

RESULT VulkanHAL::Kill() {
	RESULT r = R::OK;

	if (m_fEnableValidationLayers) {
		CRM(DestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkDebugMessenger, nullptr),
			"Failed to destroy debug messenger");
	}

	vkDestroyInstance(m_vkInstance, nullptr);
	m_vkInstance = nullptr;

Error:
	return r;
}

RESULT VulkanHAL::InitializeInstance() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;

	EPRef<SandboxWindowProcess> pSBWindowProcess = GetSBWindowProcess();
	CNM(pSBWindowProcess, "Instance initialization needs a valid sandbox window process");

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
	if (m_fEnableValidationLayers) {
		m_vkInstanceCreateInfo.enabledLayerCount = (uint32_t)m_vkValidationLayers.size();
		m_vkInstanceCreateInfo.ppEnabledLayerNames = m_vkValidationLayerNames;

		// Set up the Debug Messenger
		CRM(InitializeDebugMessenger(false), "Failed to initialize Debug Messenger");
		m_vkInstanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&m_vkDebugMessangerCreateInfo;
	}
	else {
		m_vkInstanceCreateInfo.enabledLayerCount = 0;
		m_vkInstanceCreateInfo.ppEnabledLayerNames = nullptr;
	}

	CVKRM(vkCreateInstance(&m_vkInstanceCreateInfo, nullptr, &m_vkInstance), "vkCreateInstance failed: %s", VkErrorString(vkr));
	CNM(m_vkInstance, "Vulkan Instance not created properly");

Error:
	return r;
}

RESULT VulkanHAL::InitializeDebugMessenger(bool fCreate) {
	RESULT r = R::OK;

	if (m_fEnableValidationLayers == false) {
		return R::SKIPPED;
	}

	m_vkDebugMessangerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	m_vkDebugMessangerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	m_vkDebugMessangerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	m_vkDebugMessangerCreateInfo.pfnUserCallback = VKDebugCallback;
	m_vkDebugMessangerCreateInfo.pUserData = (void*)(this);

	if (fCreate == true) {
		CRM(CreateDebugUtilsMessengerEXT(m_vkInstance, &m_vkDebugMessangerCreateInfo, nullptr, &m_vkDebugMessenger),
			"Failed to creat VK debug uitls messenger");
	}

Error:
	return r;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanHAL::VKDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
	VkDebugUtilsMessageTypeFlagsEXT msgType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) 
{
	RESULT r = R::OK;

	VulkanHAL* pVKHAL = reinterpret_cast<VulkanHAL*>(pUserData);
	CNM(pVKHAL, "VulkanHAL not valid");

	if (msgSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		DEBUG_LINEOUT("Validation layer: %s", pCallbackData->pMessage);
	}

Error:
	return VK_FALSE;
}