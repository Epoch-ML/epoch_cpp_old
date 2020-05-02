#include "VKHAL.h"

#include <string>

#include "core/types/EPRef.h"
#include "sandbox/SandboxWindowProcess.h"
#include "sandbox/win64/Win64SandboxWindowProcess.h"

#include "VulkanUtilities.h"

RESULT VKHAL::EnumerateInstanceExtensions() {
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

	for (auto& reqExtension : m_vkRequiredInstanceExtensions) {
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

RESULT VKHAL::EnumerateValidationLayers() {
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

RESULT VKHAL::Initialize(void) {
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

	CRM(InitializeWindowSurface(), "Failed to initialize window surface");

	CRM(InitializePhysicalDevice(), "Failed to initialize physical device");

	CRM(InitializeLogicalDevice(), "Failed to initialize physical device");

	CRM(InitializeSwapchain(), "Failed to initialize swapchain");

	CRM(InitializePipeline(), "Failed to initialize pipeline");

	// TODO: One of these things is not like the others
	CRM(m_pVKSwapchain->InitializeFramebuffers(m_pVKPipeline), "Failed to initialize framebuffers");

	CRM(InitializeCommandPool(), "Failed to initialize command pool");

	CRM(InitializeSemaphores(), "Failed to initialize semaphores");

Error:
	return r;
}

RESULT VKHAL::Kill(void) {
	RESULT r = R::OK;

	if (m_fEnableValidationLayers) {
		CRM(DestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkDebugMessenger, nullptr),
			"Failed to destroy debug messenger");
	}

	if (m_vkSemaphoseImageAvailable != nullptr) {
		vkDestroySemaphore(m_vkLogicalDevice, m_vkSemaphoseImageAvailable, nullptr);
		m_vkSemaphoseImageAvailable = nullptr;
	}

	if (m_vkSemaphoreRenderFinished != nullptr) {
		vkDestroySemaphore(m_vkLogicalDevice, m_vkSemaphoreRenderFinished, nullptr);
		m_vkSemaphoreRenderFinished = nullptr;
	}

	m_pVKCommandPool = nullptr;

	m_pVKSwapchain->KillFramebuffers();

	m_pVKPipeline = nullptr;

	m_pVKSwapchain = nullptr;

	if (m_vkLogicalDevice != nullptr) {
		vkDestroyDevice(m_vkLogicalDevice, nullptr);
		m_vkLogicalDevice = nullptr;
	}

	if (m_vkSurface != nullptr) {
		vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
		m_vkSurface = nullptr;
	}

	if (m_vkInstance != nullptr) {
		vkDestroyInstance(m_vkInstance, nullptr);
		m_vkInstance = nullptr;
	}

Error:
	return r;
}

RESULT VKHAL::Render(void) {
	RESULT r = R::OK;

	uint32_t imageIndex;
	VkSubmitInfo vkSubmitInfo{};
	VkSemaphore vkWaitSemaphores[] = { m_vkSemaphoseImageAvailable };
	VkPipelineStageFlags vkPipelineStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore vkSignalSemaphores[] = { m_vkSemaphoreRenderFinished };

	CVKRM(vkAcquireNextImageKHR(m_vkLogicalDevice, m_pVKSwapchain->GetVKSwapchainHandle(), UINT64_MAX, m_vkSemaphoseImageAvailable, nullptr, &imageIndex),
		"Failed to acquire next image from swapchain");

	vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.waitSemaphoreCount = 1;
	vkSubmitInfo.pWaitSemaphores = vkWaitSemaphores;
	vkSubmitInfo.pWaitDstStageMask = vkPipelineStageFlags;
	vkSubmitInfo.commandBufferCount = 1;
	vkSubmitInfo.pCommandBuffers = m_pVKCommandPool->GetCommandBufferHandle(imageIndex);  //&commandBuffers[imageIndex];
	vkSubmitInfo.signalSemaphoreCount = 1;
	vkSubmitInfo.pSignalSemaphores = vkSignalSemaphores;

	CVKRM(vkQueueSubmit(m_vkGraphicsQueueHandle, 1, &vkSubmitInfo, VK_NULL_HANDLE),
		"Failed to submit to graphics queue");

	CVKRM(vkDeviceWaitIdle(m_vkLogicalDevice), "Failed to wait idle");

Error:
	return r;
}

RESULT VKHAL::InitializeSemaphores() {
	RESULT r = R::OK;

	VkSemaphoreCreateInfo vkSempahoreCreateInfo = {};
	vkSempahoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	CVKRM(vkCreateSemaphore(m_vkLogicalDevice, &vkSempahoreCreateInfo, nullptr, &m_vkSemaphoseImageAvailable),
		"Failed to create image available semaphore");

	CVKRM(vkCreateSemaphore(m_vkLogicalDevice, &vkSempahoreCreateInfo, nullptr, &m_vkSemaphoreRenderFinished),
		"Failed to create render finished semaphore");

Error:
	return r;
}

RESULT VKHAL::InitializeInstance() {
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

bool VKHAL::CheckPhysicalDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice) {
	RESULT r = R::OK;

	uint32_t vkPhysicalDeviceExtensionCount;
	CVKRM(vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &vkPhysicalDeviceExtensionCount, nullptr),
		"Failed to enumerate physical device extensions");

	{

		EPVector<VkExtensionProperties> vkAvailablePhysicaDeviceExtensions(vkPhysicalDeviceExtensionCount);

		CVKRM(vkEnumerateDeviceExtensionProperties(
			vkPhysicalDevice,
			nullptr,
			&vkPhysicalDeviceExtensionCount,
			vkAvailablePhysicaDeviceExtensions.data(vkPhysicalDeviceExtensionCount)),
		"Failed to enumerate physical device extensions");

		// Make sure all required extensions are in the supported physical device extensions
		EPVector<VkExtensionProperties> vkRequiredPhyscalDeviceExtensions = m_vkExtensions;
		for (auto& strVKRequiredPhysicalDeviceExtension : m_vkRequiredPhysicalDeviceExtensions) {
			bool fFound = false;

			for (auto& vkPhysicalDeviceExtension : vkAvailablePhysicaDeviceExtensions) {
				if (strcmp(vkPhysicalDeviceExtension.extensionName, strVKRequiredPhysicalDeviceExtension) == 0) {
					fFound = true;
					break;
				}
			}

			if (fFound == false) {
				DEBUG_LINEOUT("Required extension: %s not found in physical device",
					strVKRequiredPhysicalDeviceExtension);
				return false;
			}
		}
	}

Success:
	return true;

Error:
	return false;
}

// TODO: We might want to keep this data 
bool VKHAL::IsVKPhysicalDeviceSuitable(VkPhysicalDevice vkPhysicalDevice) {
	VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;

	vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkPhysicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &vkPhysicalDeviceFeatures);

	if (vkPhysicalDeviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		return false;

	if (vkPhysicalDeviceFeatures.geometryShader == false) 
		return false;

	auto queueFamilies = EnumerateVKPhysicalDeviceQueueFamilies(vkPhysicalDevice);
	if (queueFamilies.size() == 0)
		return false;

	// Check for VK_QUEUE_GRAPHICS_BIT
	bool fFoundQueueGraphicsBit = false;
	for (auto& family : queueFamilies) {
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			fFoundQueueGraphicsBit = true;
			break;
		}
	}

	if (fFoundQueueGraphicsBit == false)
		return false;

	// Check extension support
	if (CheckPhysicalDeviceExtensionSupport(vkPhysicalDevice) == false) 
		return false;

	// Check swap chain
	EPRef<VKSwapchain> pVKSwapchain = VKSwapchain::make(vkPhysicalDevice, m_vkSurface);
	if (pVKSwapchain == nullptr) {
		DEBUG_LINEOUT("Failed to allocate VK swapchain");
		return false;
	}

	if (pVKSwapchain->SurfaceFormats().empty())
		return false;

	if (pVKSwapchain->PresentationModes().empty())
		return false;


	return true;
}


RESULT VKHAL::EnumeratePhysicalDevices() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;

	CVKRM(vkEnumeratePhysicalDevices(m_vkInstance, &m_vkPhysicalDeviceCount, nullptr),
		"Failed to enumerate physical devices");

	CBM(m_vkPhysicalDeviceCount != 0, "There are zero vulkan devices present");

	m_vkAvailablePhysicalDevices = EPVector<VkPhysicalDevice>(m_vkPhysicalDeviceCount);
	CVKRM(vkEnumeratePhysicalDevices(
		m_vkInstance, 
		&m_vkPhysicalDeviceCount, 
		m_vkAvailablePhysicalDevices.data(m_vkPhysicalDeviceCount)),
	"Failed to enumerate physical devices");

	// Collect suitable devices
	for (auto& device : m_vkAvailablePhysicalDevices) {
		if (IsVKPhysicalDeviceSuitable(device)) {
			m_vkSuitablePhysicalDevices.PushBack(device);
		}
	}

Error:
	return r;
}

RESULT VKHAL::InitializePhysicalDevice() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;

	CRM(EnumeratePhysicalDevices(), "Failed to enumerate vulkan physical devices");

	CBM(m_vkSuitablePhysicalDevices.size() != 0, "Found no suitable vulkan physical devives");

	// TODO: Probably want to do something with other suitable devices
	// or choose more wisely
	m_vkPhysicalDevice = m_vkSuitablePhysicalDevices[0];

	CNM(m_vkPhysicalDevice, "Physical device cannot be null");
	
	vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_vkPhysicalDeviceFeatures);

Error:
	return r;
}

RESULT VKHAL::InitializeLogicalDevice() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;
	
	EPVector<uint32_t> familyQueueIndexes;
	uint32_t graphicsFamilyQueueIndex;
	uint32_t presentFamilyQueueIndex;

	CNM(m_vkPhysicalDevice, "Cannot initialize logical device without a valid physical device");
	CNM(m_vkSurface, "Cannot initialize logical device without a valid surface");

	familyQueueIndexes = FindQueueFamilies(m_vkPhysicalDevice, m_vkSurface);
	 
	for (auto& queueFamilyIndex : familyQueueIndexes) {

		VkDeviceQueueCreateInfo vkDeviceQueueCreateInfo = {};

		vkDeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		vkDeviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		vkDeviceQueueCreateInfo.queueCount = 1;
		
		float queuePriority;
		queuePriority = 1.0f;
		vkDeviceQueueCreateInfo.pQueuePriorities = &queuePriority;

		m_vkDeviceQueueCreateInfos.PushBack(vkDeviceQueueCreateInfo);
	}
	
	// VkDeviceCreateInfo

	m_vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	m_vkDeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(m_vkDeviceQueueCreateInfos.size());
	m_vkDeviceCreateInfo.pQueueCreateInfos = m_vkDeviceQueueCreateInfos.data();
	m_vkDeviceCreateInfo.pEnabledFeatures = &m_vkPhysicalDeviceFeatures;

	m_vkDeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_vkRequiredPhysicalDeviceExtensions.size());
	m_vkDeviceCreateInfo.ppEnabledExtensionNames = (char **)(m_vkRequiredPhysicalDeviceExtensions.data());

	if (m_fEnableValidationLayers) {
		m_vkDeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_vkValidationLayers.size());
		m_vkDeviceCreateInfo.ppEnabledLayerNames = m_vkValidationLayerNames;
	}
	else {
		m_vkDeviceCreateInfo.enabledLayerCount = 0;
		m_vkDeviceCreateInfo.ppEnabledLayerNames = nullptr;
	}

	CVKRM(vkCreateDevice(m_vkPhysicalDevice, &m_vkDeviceCreateInfo, nullptr, &m_vkLogicalDevice),
		"Failed to create logitcal device");
	CNM(m_vkLogicalDevice, "Failed to create logical vulkan device");

	// TODO: If the family queue index is the same the handle is equivalent
	graphicsFamilyQueueIndex = familyQueueIndexes[0];
	vkGetDeviceQueue(m_vkLogicalDevice, graphicsFamilyQueueIndex, 0, &m_vkGraphicsQueueHandle);
	CNM(m_vkGraphicsQueueHandle, "Failed to retrieve graphics queue handle");

	presentFamilyQueueIndex = (familyQueueIndexes.size() > 1) ? familyQueueIndexes[1] : familyQueueIndexes[0];
	vkGetDeviceQueue(m_vkLogicalDevice, presentFamilyQueueIndex, 0, &m_vkPresentationQueueHandle);
	CNM(m_vkPresentationQueueHandle, "Failed to retrieve presentation queue handle");

Error:
	return r;
}

RESULT VKHAL::InitializeWindowSurface() {
	RESULT r = R::OK;
	VkResult vkr = VK_SUCCESS;

	m_vkWin32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	m_vkWin32SurfaceCreateInfo.hwnd = (HWND)(GetSBWindowProcess()->GetNativeWindowHandle());
	m_vkWin32SurfaceCreateInfo.hinstance = GetModuleHandle(nullptr);

	CVKRM(vkCreateWin32SurfaceKHR(m_vkInstance, &m_vkWin32SurfaceCreateInfo, nullptr, &m_vkSurface),
		"Failed to create win32 windows surface");

Error:
	return r;
}

RESULT VKHAL::InitializeSwapchain() {
	RESULT r = R::OK;

	CNM(m_vkPhysicalDevice, "Swapchain needs valid physical device");
	CNM(m_vkSurface, "Swapchain needs valid surface");
	CNM(m_vkLogicalDevice, "Swapchain needs valid logical device");

	m_pVKSwapchain = VKSwapchain::make(
		m_vkPhysicalDevice, 
		m_vkSurface, 
		m_vkLogicalDevice,
		VK_FORMAT_B8G8R8A8_SRGB, 
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		VK_PRESENT_MODE_FIFO_KHR,
		{ (uint32_t)GetSBWindowProcess()->GetWidth(), (uint32_t)GetSBWindowProcess()->GetHeight() }
	);

	CNM(m_pVKSwapchain, "Failed to make vk swapchain");

Error:
	return r;
}

RESULT VKHAL::InitializePipeline() {
	RESULT r = R::OK;

	//CNM(m_vkPhysicalDevice, "Swapchain needs valid physical device");
	//CNM(m_vkSurface, "Swapchain needs valid surface");
	CNM(m_vkLogicalDevice, "Pipeline needs valid logical device");
	CNM(m_pVKSwapchain, "Pipeline needs valid swapchain");

	m_pVKPipeline = VKPipeline::make(m_vkLogicalDevice, m_pVKSwapchain);
	CNM(m_pVKPipeline, "Failed to make vk pipeline");

Error:
	return r;
}

RESULT VKHAL::InitializeCommandPool() {
	RESULT r = R::OK;

	CNM(m_vkPhysicalDevice, "Command pool needs valid physical device");
	CNM(m_vkSurface, "Command pool needs valid surface");
	CNM(m_vkLogicalDevice, "Command pool needs valid logical device");
	CNM(m_pVKSwapchain, "Command pool needs valid swapchain");

	m_pVKCommandPool = VKCommandPool::make(m_vkPhysicalDevice, m_vkLogicalDevice, m_vkSurface, m_pVKPipeline, m_pVKSwapchain);
	CNM(m_pVKCommandPool, "Failed to make vk command pool");

Error:
	return r;
}


RESULT VKHAL::InitializeDebugMessenger(bool fCreate) {
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

VKAPI_ATTR VkBool32 VKAPI_CALL VKHAL::VKDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
	VkDebugUtilsMessageTypeFlagsEXT msgType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) 
{
	RESULT r = R::OK;

	VKHAL* pVKHAL = reinterpret_cast<VKHAL*>(pUserData);
	CNM(pVKHAL, "VulkanHAL not valid");

	if (msgSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		DEBUG_LINEOUT("Validation layer: %s", pCallbackData->pMessage);
	}

Error:
	return VK_FALSE;
}