#ifndef VULKAN_HAL_H_
#define VULKAN_HAL_H_

#include "core/ehm/ehm.h"

// epoch Vulkan HAL
// epoch/src/hal/vulkan/VulkanHAL.h

#include "hal/hal.h"
#include <vulkan/vulkan.h>

#include "core/types/EPTuple.h"

// TODO: split off the platform specific implementation
#include <vulkan/vulkan_win32.h>

#include "VKSwapchain.h"
#include "VKPipeline.h"
#include "VKCommandPool.h"

class VKHAL :
	public HAL
{

public:
	VKHAL() {
		//
	}

protected:
	virtual ~VKHAL() override {
		//
	}

public:
	virtual RESULT Initialize(void) override;
	virtual RESULT Kill(void) override;
	virtual RESULT Render(void) override;

	virtual HAL::type GetType() override {
		return HAL::type::vulkan;
	}

private:
// Instance 
	RESULT InitializeInstance();

	const EPVector<EPTuple<int, const char*>> m_vkRequiredInstanceExtensions = {
		{VK_KHR_surface, VK_KHR_SURFACE_EXTENSION_NAME},
		{VK_KHR_win32_surface, VK_KHR_WIN32_SURFACE_EXTENSION_NAME},
		{VK_EXT_debug_utils, VK_EXT_DEBUG_UTILS_EXTENSION_NAME}
	};

	RESULT EnumerateInstanceExtensions();

// Validation Layers
	const EPVector<const char*> m_vkRequiredValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	RESULT EnumerateValidationLayers();

// Physical Device
	const EPVector<const char*> m_vkRequiredPhysicalDeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	RESULT EnumeratePhysicalDevices();
	RESULT InitializePhysicalDevice();

	bool CheckPhysicalDeviceExtensionSupport(VkPhysicalDevice vkPhysicalDevice);
	bool IsVKPhysicalDeviceSuitable(VkPhysicalDevice vkPhysicalDevice);

	VkPhysicalDevice GetPhysicalDevice() { return m_vkPhysicalDevice; }

// Logical Device
	RESULT InitializeLogicalDevice();
	VkDevice GetLogicalDevice() { return m_vkLogicalDevice; }

// Window Surface
	RESULT InitializeWindowSurface();
	VkSurfaceKHR GetSurface() { return m_vkSurface; }

// Swapchain
	RESULT InitializeSwapchain();

// Pipeline
	RESULT InitializePipeline();

// Command pool
	RESULT InitializeCommandPool();

// Debugging
	RESULT InitializeDebugMessenger(bool fCreate);

	static VKAPI_ATTR VkBool32 VKAPI_CALL VKDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
		VkDebugUtilsMessageTypeFlagsEXT msgType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);

// Semaphores
	RESULT InitializeSemaphores();

private:
	VkInstance m_vkInstance;
	VkApplicationInfo m_vkApplicationInfo = {};
	VkInstanceCreateInfo m_vkInstanceCreateInfo = {};

// Instance Extensions
	uint32_t m_vkEnumeratedExtensionCount = 0;
	EPVector<VkExtensionProperties> m_vkEnumeratedExtensions;
	EPVector<VkExtensionProperties> m_vkExtensions;
	const char* m_vkExtensionNames[64] = { 0 };		// TODO: not a huge fan of the duplication here

// Validation Layers
	uint32_t m_vkValidationLayerCount;
	EPVector<VkLayerProperties> m_vkAvailableValidationLayers;
	EPVector<VkLayerProperties> m_vkValidationLayers;
	const char* m_vkValidationLayerNames[64] = { 0 };

// Physical Device
	uint32_t m_vkPhysicalDeviceCount = 0;
	EPVector<VkPhysicalDevice> m_vkAvailablePhysicalDevices;
	EPVector<VkPhysicalDevice> m_vkSuitablePhysicalDevices;
	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkPhysicalDeviceFeatures m_vkPhysicalDeviceFeatures{};
	VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;

// Logical Device
	EPVector<VkDeviceQueueCreateInfo> m_vkDeviceQueueCreateInfos;
	VkDeviceCreateInfo m_vkDeviceCreateInfo{};

	VkDevice m_vkLogicalDevice = nullptr;
	VkQueue m_vkGraphicsQueueHandle = nullptr;
	VkQueue m_vkPresentationQueueHandle = nullptr;

// Window Surface
	VkSurfaceKHR m_vkSurface = nullptr;
	VkWin32SurfaceCreateInfoKHR m_vkWin32SurfaceCreateInfo = {};

// Swapchain
	EPRef<VKSwapchain> m_pVKSwapchain = nullptr;

// Pipeline
	EPRef<VKPipeline> m_pVKPipeline = nullptr;

// Command pool
	EPRef<VKCommandPool> m_pVKCommandPool = nullptr;

// Debugging
	VkDebugUtilsMessengerEXT m_vkDebugMessenger = {};
	VkDebugUtilsMessengerCreateInfoEXT m_vkDebugMessangerCreateInfo;

// Rendering semaphores
	VkSemaphore m_vkSemaphoseImageAvailable;
	VkSemaphore m_vkSemaphoreRenderFinished;

#ifdef NDEBUG
	const bool m_fEnableValidationLayers = false;
#else
	const bool m_fEnableValidationLayers = true;
#endif
};


#endif // ! VULKAN_HAL_H_