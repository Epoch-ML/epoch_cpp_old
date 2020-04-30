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

class VulkanHAL :
	public HAL
{

public:
	VulkanHAL();

protected:
	virtual ~VulkanHAL() override;

public:
	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

	virtual HAL::type GetType() override {
		return HAL::type::vulkan;
	}

private:
// Instance Extensions
	RESULT InitializeInstance();

	const EPVector<EPTuple<int, const char*>> m_RequiredExtensions = {
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
	RESULT EnumeratePhysicalDevices();
	RESULT InitializePhysicalDevice();
	bool IsVKPhysicalDeviceSuitable(VkPhysicalDevice vkPhysicalDevice);
	EPVector<VkQueueFamilyProperties> EnumerateVKPhysicalDeviceQueueFamilies(VkPhysicalDevice vkPhysicalDevice);

// Logical Device
	RESULT InitializeLogicalDevice();

// Window Surface
	RESULT InitializeWindowSurface();

// Debugging
	RESULT InitializeDebugMessenger(bool fCreate);

	static VKAPI_ATTR VkBool32 VKAPI_CALL VKDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity,
		VkDebugUtilsMessageTypeFlagsEXT msgType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);

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
	VkDeviceQueueCreateInfo m_vkDeviceQueueCreateInfo{};
	VkDeviceCreateInfo m_vkDeviceCreateInfo{};

	VkDevice m_vkLogicalDevice = nullptr;
	VkQueue m_vkQueueHandle = nullptr;

// Window Surface
	VkSurfaceKHR m_vkSurface = nullptr;
	VkWin32SurfaceCreateInfoKHR m_vkWin32SurfaceCreateInfo = {};

// Debugging
	VkDebugUtilsMessengerEXT m_vkDebugMessenger = {};
	VkDebugUtilsMessengerCreateInfoEXT m_vkDebugMessangerCreateInfo;

#ifdef NDEBUG
	const bool m_fEnableValidationLayers = false;
#else
	const bool m_fEnableValidationLayers = true;
#endif
};


#endif // ! VULKAN_HAL_H_