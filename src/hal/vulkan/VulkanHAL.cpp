#include "VulkanHAL.h"

#include "core/types/EPRef.h"
#include "sandbox/SandboxWindowProcess.h"
#include "sandbox/win64/Win64SandboxWindowProcess.h"

#include "VulkanUtilities.h"

VulkanHAL::VulkanHAL() {
	//
}

VulkanHAL::~VulkanHAL() {
	//
}

RESULT VulkanHAL::EnumerateInstanceExtensions() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;

	vkr = vkEnumerateInstanceExtensionProperties(nullptr, &m_extensionCount, nullptr);
	CBRM(RSUCCESS(vkr), (RESULT)(vkr), "vkEnumerateInstanceExtensionProperties failed: %s", VkErrorString(vkr));
	CBM(m_extensionCount != 0, "vkEnumerateInstanceExtensionProperties resulted in zero extensions");

	m_extensions = EPVector<VkExtensionProperties>(m_extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &m_extensionCount, m_extensions.data());

Error:
	return r;
}

RESULT VulkanHAL::Initialize() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;

	VkApplicationInfo vkApplicationInfo{};
	VkInstanceCreateInfo vkInstanceCreateInfo{};

	DEBUG_LINEOUT("Initializing Vulkan");

	EPRef<SandboxWindowProcess> pSBWindowProcess = GetSBWindowProcess();
	CNM(pSBWindowProcess, "Initialization needs a valid sandbox window process");

	CRM(EnumerateInstanceExtensions(), "Failed to enumerate VK extensions");

	DEBUG_LINEOUT("Extensions Supported: %d", m_extensionCount);

	//VkApplicationInfo vkApplicationInfo{};

	vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkApplicationInfo.pApplicationName = pSBWindowProcess->GetName().c_str();
	vkApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	vkApplicationInfo.pEngineName = "No Engine";
	vkApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	vkApplicationInfo.apiVersion = VK_API_VERSION_1_0;

	//VkInstanceCreateInfo vkInstanceCreateInfo{};

	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pApplicationInfo = &vkApplicationInfo;
	
	vkInstanceCreateInfo.enabledExtensionCount = m_extensionCount;
	//vkInstanceCreateInfo.ppEnabledExtensionNames = (const char **)(m_extensions.GetCBuffer());
	vkInstanceCreateInfo.ppEnabledExtensionNames = nullptr;
	
	vkInstanceCreateInfo.enabledLayerCount = 0;
	vkInstanceCreateInfo.ppEnabledLayerNames = nullptr;

	vkr = vkCreateInstance(&vkInstanceCreateInfo, nullptr, &m_vkInstance);
	CBRM(RSUCCESS(vkr), (RESULT)(vkr), "vkCreateInstance failed: %s", VkErrorString(vkr));

Error:
	return r;
}