#include "VKHAL.h"

#include <string>

#include "core/types/EPRef.h"
#include "sandbox/SandboxWindowProcess.h"
#include "sandbox/win64/Win64SandboxWindowProcess.h"

#include "VulkanUtilities.h"

#include "VKBuffer.h"
#include "VKVertex.h"

#include "VKSwapchain.h"
#include "VKPipeline.h"

#include "VKCommandPool.h"
#include "VKCommandBuffers.h"
#include "VKVertexBuffer.h"
#include "VKTexture.h"
#include "VKModel.h"

#include "VKDescriptorSet.h"
#include "VKDepthAttachment.h"

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

	CRM(InitializeConcurrencyPrimitives(), "Failed to initialize semaphores");

Error:
	return r;
}

RESULT VKHAL::Kill(void) {
	RESULT r = R::OK;

	if (m_fEnableValidationLayers) {
		CRM(DestroyDebugUtilsMessengerEXT(m_vkInstance, m_vkDebugMessenger, nullptr),
			"Failed to destroy debug messenger");
	}

	CRM(CleanupSwapchain(), "Failed to clean up swapchain");

	for (int i = 0; i < k_MaxConcurrentFrames; i++) {
		if (m_imageAvailableVkSemaphores[i] != nullptr) {
			vkDestroySemaphore(m_vkLogicalDevice, m_imageAvailableVkSemaphores[i], nullptr);
			m_imageAvailableVkSemaphores[i] = nullptr;
		}

		if (m_renderFinishedVkSemaphores[i] != nullptr) {
			vkDestroySemaphore(m_vkLogicalDevice, m_renderFinishedVkSemaphores[i], nullptr);
			m_renderFinishedVkSemaphores[i] = nullptr;
		}

		if (m_concurrentFrameFences[i] != nullptr) {
			vkDestroyFence(m_vkLogicalDevice, m_concurrentFrameFences[i], nullptr);
			m_concurrentFrameFences[i] = nullptr;
		}
	}

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
	VkResult vkr = VK_SUCCESS;

	uint32_t imageIndex;
	VkSubmitInfo vkSubmitInfo{};
	VkSemaphore vkWaitSemaphores[] = { m_imageAvailableVkSemaphores[m_currentFrame] };
	VkPipelineStageFlags vkPipelineStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore vkSignalSemaphores[] = { m_renderFinishedVkSemaphores[m_currentFrame] };
	VkPresentInfoKHR vkPresentInfo{};
	VkSwapchainKHR vkSwapchains[] = { m_pVKSwapchain->GetVKSwapchainHandle() };
	
	// Wait on fences
	CVKR(vkWaitForFences(m_vkLogicalDevice, 1, &m_concurrentFrameFences[m_currentFrame], VK_TRUE, UINT64_MAX));	

	vkr = vkAcquireNextImageKHR(
		m_vkLogicalDevice,
		m_pVKSwapchain->GetVKSwapchainHandle(),
		UINT64_MAX,
		m_imageAvailableVkSemaphores[m_currentFrame],
		nullptr,
		&imageIndex
	);
		
	// Check for out-of-date swapchain
	if (vkr == VK_ERROR_OUT_OF_DATE_KHR) {
		CRM(InitializeSwapchain(), "Failed to reinitialize swapchain");
		return r;
	}
	else if (vkr != VK_SUCCESS && vkr != VK_SUBOPTIMAL_KHR) {
		CVKRM(vkr, "Failed to acquire next image from swapchain")
	}

	// Make sure we don't overwrite the current swapchain image / frame
	if (m_currentlyUsedFrameFences[imageIndex] != nullptr) {
		CVKR(vkWaitForFences(m_vkLogicalDevice, 1, &m_currentlyUsedFrameFences[imageIndex], VK_TRUE, UINT64_MAX));
	}
	m_currentlyUsedFrameFences[imageIndex] = m_concurrentFrameFences[m_currentFrame];

	// Update uniform buffers 
	m_pVKPipeline->Update(imageIndex);

	// This submits the command buffer itself
	vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	vkSubmitInfo.waitSemaphoreCount = 1;
	vkSubmitInfo.pWaitSemaphores = vkWaitSemaphores;
	vkSubmitInfo.pWaitDstStageMask = vkPipelineStageFlags;
	vkSubmitInfo.commandBufferCount = 1;
	vkSubmitInfo.pCommandBuffers = m_pVKCommandBuffers->GetCommandBufferHandle(imageIndex);		
	vkSubmitInfo.signalSemaphoreCount = 1;
	vkSubmitInfo.pSignalSemaphores = vkSignalSemaphores;

	// Reset the fence for the current frame
	CVKR(vkResetFences(m_vkLogicalDevice, 1, &m_concurrentFrameFences[m_currentFrame]));

	CVKRM(vkQueueSubmit(m_vkGraphicsQueueHandle, 1, &vkSubmitInfo, m_concurrentFrameFences[m_currentFrame]),
		"Failed to submit to graphics queue");

	// Presentation info
	vkPresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	vkPresentInfo.waitSemaphoreCount = 1;
	vkPresentInfo.pWaitSemaphores = vkSignalSemaphores;
	vkPresentInfo.swapchainCount = 1;
	vkPresentInfo.pSwapchains = vkSwapchains;
	vkPresentInfo.pImageIndices = &imageIndex;

	// Handle suboptimal swapchain
	vkr = vkQueuePresentKHR(m_vkPresentationQueueHandle, &vkPresentInfo);
	if (vkr == VK_ERROR_OUT_OF_DATE_KHR || vkr == VK_SUBOPTIMAL_KHR || m_fPendingSwapchainResize) {
		m_fPendingSwapchainResize = false;
		CRM(InitializeSwapchain(), "Failed to reinitialize swapchain");
	}
	else if (vkr != VK_SUCCESS) {
		CVKRM(vkr, "Failed to present to presentation queue");
	}


	m_currentFrame = (m_currentFrame + 1) % k_MaxConcurrentFrames;

Error:
	return r;
}

RESULT VKHAL::OnResize(uint32_t width, uint32_t height) {
	RESULT r = R::OK;

	m_fPendingSwapchainResize = true;

Error:
	return r;
}

RESULT VKHAL::WaitForIdle(void) {
	RESULT r = R::OK;

	CVKRM(vkDeviceWaitIdle(m_vkLogicalDevice), "Failed to wait idle");

Error:
	return r;
}

RESULT VKHAL::InitializeConcurrencyPrimitives() {
	RESULT r = R::OK;

	m_imageAvailableVkSemaphores = EPVector<VkSemaphore>(k_MaxConcurrentFrames, true);
	m_renderFinishedVkSemaphores = EPVector<VkSemaphore>(k_MaxConcurrentFrames, true);
	m_concurrentFrameFences = EPVector<VkFence>(k_MaxConcurrentFrames, true);
	m_currentlyUsedFrameFences = EPVector<VkFence>(m_pVKSwapchain->GetSwapchainImageCount(), nullptr);

	VkSemaphoreCreateInfo vkSempahoreCreateInfo = {};
	vkSempahoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo vkFenceCreateInfo{};
	vkFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vkFenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < k_MaxConcurrentFrames; i++) {
		CVKRM(vkCreateSemaphore(m_vkLogicalDevice, &vkSempahoreCreateInfo, nullptr, &m_imageAvailableVkSemaphores[i]),
			"Failed to create image available semaphore %d", i);

		CVKRM(vkCreateSemaphore(m_vkLogicalDevice, &vkSempahoreCreateInfo, nullptr, &m_renderFinishedVkSemaphores[i]),
			"Failed to create render finished semaphore %d", i);

		CVKRM(vkCreateFence(m_vkLogicalDevice, &vkFenceCreateInfo, nullptr, &m_concurrentFrameFences[i]),
			"Faioled to create current frame fence %d", i);
	}

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

	// Device Properties
	vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkPhysicalDeviceProperties);

	if (vkPhysicalDeviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		return false;

	// FEatures
	vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &vkPhysicalDeviceFeatures);

	if (vkPhysicalDeviceFeatures.geometryShader == false) 
		return false;

	if (vkPhysicalDeviceFeatures.samplerAnisotropy == false)
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
	EPRef<VKSwapchain> pVKSwapchain = VKSwapchain::make(vkPhysicalDevice, m_vkSurface, m_pVKCommandPool);
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
	
	VKQueueFamilies vkQueueFamilies;

	CNM(m_vkPhysicalDevice, "Cannot initialize logical device without a valid physical device");
	CNM(m_vkSurface, "Cannot initialize logical device without a valid surface");

	vkQueueFamilies = FindQueueFamilies(m_vkPhysicalDevice, m_vkSurface);
	 
	for (auto& queueFamilyIndex : vkQueueFamilies.GetUniqueIndexes()) {

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

	if (vkQueueFamilies.HasGraphicsQueue()) {
		vkGetDeviceQueue(m_vkLogicalDevice, vkQueueFamilies.GetGraphicsQueueIndex(), 0, &m_vkGraphicsQueueHandle);
		CNM(m_vkGraphicsQueueHandle, "Failed to retrieve graphics queue handle");
	}

	if (vkQueueFamilies.HasPresentationQueue()) {
		vkGetDeviceQueue(m_vkLogicalDevice, vkQueueFamilies.GetPresentationQueueIndex(), 0, &m_vkPresentationQueueHandle);
		CNM(m_vkPresentationQueueHandle, "Failed to retrieve presentation queue handle");
	}

	if (vkQueueFamilies.HasComputeQueue()) {
		vkGetDeviceQueue(m_vkLogicalDevice, vkQueueFamilies.GetComputeQueueIndex(), 0, &m_vkComputeQueueHandle);
		CNM(m_vkComputeQueueHandle, "Failed to retrieve compute queue handle");
	}

	if (vkQueueFamilies.HasTransferQueue()) {
		vkGetDeviceQueue(m_vkLogicalDevice, vkQueueFamilies.GetTransferQueueIndex(), 0, &m_vkTransferQueueHandle);
		CNM(m_vkTransferQueueHandle, "Failed to retrieve transfer queue handle");
	}

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

	CVKR(vkDeviceWaitIdle(m_vkLogicalDevice));

	CRM(CleanupSwapchain(), "Failed to clean up swapchain first");

	// Initialize Command Pool first
	CRM(InitializeCommandPool(), "Failed to initialize command pool");

	CNM(m_pVKCommandPool, "Swapchain initialization requires valid command pool");

	m_pVKSwapchain = VKSwapchain::make(
		m_vkPhysicalDevice, 
		m_vkSurface, 
		m_pVKCommandPool,
		m_vkLogicalDevice,
		VK_FORMAT_B8G8R8A8_SRGB, 
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		VK_PRESENT_MODE_FIFO_KHR,
		{ (uint32_t)GetSBWindowProcess()->GetWidth(), (uint32_t)GetSBWindowProcess()->GetHeight() }
	);

	CNM(m_pVKSwapchain, "Failed to make vk swapchain");


	// Texture before pipeline?
	CRM(InitializeTexture(), "Failed to initialize texture");	// TODO: yea this
	
	CRM(InitializePipeline(), "Failed to initialize pipeline");

	// TODO: One of these things is not like the others
	CRM(m_pVKSwapchain->InitializeFramebuffers(m_pVKPipeline), "Failed to initialize framebuffers");

	//CRM(InitializeVertexBuffer(), "Failed to initialize vertex buffer");
	CRM(InitializeVKModel(), "Failed to initialize VK model");

	CRM(m_pVKPipeline->InitializeDescriptors(m_pVKTexture), 
		"Failed to initialize / update descriptors");

	// Command buffers AFTER descriptors (makes sense)
	CRM(InitializeCommandBuffers(), "Failed to initialize command buffers");

Error:
	return r;
}

RESULT VKHAL::CleanupSwapchain() {
	RESULT r = R::OK;

	// TODO: only destroy command buffers, not pool
	// This is an optimization and will get ironed out with better arch
	m_pVKCommandBuffers = nullptr;

	m_pVKCommandPool = nullptr;

	if (m_pVKSwapchain != nullptr) {
		m_pVKSwapchain->KillFramebuffers();
	}

	m_pVKPipeline = nullptr;

	m_pVKSwapchain = nullptr;

Error:
	return r;
}

RESULT VKHAL::InitializePipeline() {
	RESULT r = R::OK;

	//CNM(m_vkPhysicalDevice, "Swapchain needs valid physical device");
	//CNM(m_vkSurface, "Swapchain needs valid surface");
	CNM(m_vkLogicalDevice, "Pipeline needs valid logical device");
	CNM(m_pVKSwapchain, "Pipeline needs valid swapchain");

	m_pVKPipeline = VKPipeline::make(m_vkPhysicalDevice, m_vkLogicalDevice, m_pVKSwapchain);
	CNM(m_pVKPipeline, "Failed to make vk pipeline");

Error:
	return r;
}

RESULT VKHAL::InitializeTexture() {
	RESULT r = R::OK;

	CNM(m_pVKCommandPool, "Vertex buffer needs valid command pool");

	m_pVKTexture = VKTexture::make(
		m_vkPhysicalDevice, 
		m_vkLogicalDevice,
		m_pVKCommandPool,
		"wooden_crate.jpg",
		//"statue.jpg",
		m_pVKCommandPool->GetVKQueueHandle()
	);

	CNM(m_pVKTexture, "Failed to create texture");

Error:
	return r;
}

RESULT VKHAL::InitializeVertexBuffer() {
	RESULT r = R::OK;

	CNM(m_pVKCommandPool, "Vertex buffer needs valid command pool");

	// TODO: not hard coded vertex count
	m_pVKVertexBuffer = VKVertexBuffer::make(
		m_pVKCommandPool->GetVKPhyscialDeviceHandle(),
		m_pVKCommandPool->GetVKLogicalDeviceHandle(),
		m_pVKCommandPool,
		m_pVKCommandPool->GetVKQueueHandle());

	CNM(m_pVKVertexBuffer, "Failed to create vertex buffer");

Error:
	return r;
}

RESULT VKHAL::InitializeVKModel() {
	RESULT r = R::OK;

	CNM(m_pVKCommandPool, "Vertex buffer needs valid command pool");

	m_pVKModel = VKModel::make(
		m_pVKCommandPool->GetVKPhyscialDeviceHandle(),
		m_pVKCommandPool->GetVKLogicalDeviceHandle(),
		m_pVKCommandPool, 
		"viking_room.obj", 
		"viking_room.png");

	CNM(m_pVKModel, "Failed to create VK model");

Error:
	return r;
}

RESULT VKHAL::InitializeCommandBuffers() {
	RESULT r = R::OK;

	// TODO: this is probably not true in a more general pipeline 
	CNM(m_pVKCommandPool, "Command buffers need valid comman pool");
	//CNM(m_pVKVertexBuffer, "Command buffers need valid vertex buffer");

	//m_pVKCommandBuffers = m_pVKCommandPool->MakeVertexDescriptorCommandBuffers(
	//	m_pVKPipeline,
	//	m_pVKSwapchain,
	//	m_pVKVertexBuffer, 
	//	m_pVKPipeline->GetVKDescriptorSet()
	//);

	CNM(m_pVKModel, "Command buffers need valid vertex buffer");
	m_pVKCommandBuffers = m_pVKCommandPool->MakeVertexDescriptorCommandBuffers(
		m_pVKPipeline,
		m_pVKSwapchain,
		m_pVKModel, 
		m_pVKPipeline->GetVKDescriptorSet()
	);

	CNM(m_pVKCommandBuffers, "Failed to create command buffers");

Error:
	return r;
}

RESULT VKHAL::InitializeCommandPool() {
	RESULT r = R::OK;

	CNM(m_vkPhysicalDevice, "Command pool needs valid physical device");
	CNM(m_vkSurface, "Command pool needs valid surface");
	CNM(m_vkLogicalDevice, "Command pool needs valid logical device");
	CNM(m_vkGraphicsQueueHandle, "Need valid graphics queue handle");

	m_pVKCommandPool = VKCommandPool::make(
		m_vkPhysicalDevice, 
		m_vkLogicalDevice, 
		m_vkSurface, 
		m_vkGraphicsQueueHandle
	);

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