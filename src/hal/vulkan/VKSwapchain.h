#ifndef VULKAN_SWAPCHAIN_H_
#define VULKAN_SWAPCHAIN_H_

#include "core/ehm/ehm.h"

// epoch Vulkan HAL
// epoch/src/hal/vulkan/VulkanHAL.h

#include "hal/swapchain.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"

class VKPipeline;
class VKFramebuffer;

class VKSwapchain :
	public swapchain
{
private:
	VKSwapchain(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface);

	virtual RESULT Initialize() override;


	RESULT SelectSurfaceFormat(VkFormat, VkColorSpaceKHR);
	RESULT SelectPresentationMode(VkPresentModeKHR);
	RESULT SelectSwapchainExtent(VkExtent2D vkExtent2D);
	RESULT CreateSwapchain();

	virtual RESULT Kill() override;

public:
	virtual ~VKSwapchain() override;

	RESULT KillFramebuffers();
	RESULT InitializeFramebuffers(const EPRef<VKPipeline> &pVKPipeline);

	const VkExtent2D& GetSwapchainExtent() const { return m_vkSelectedExtent2D; }
	virtual uint32_t GetExtentsWidth() override { return m_vkSelectedExtent2D.width; }
	virtual uint32_t GetExtentsHeight() override { return m_vkSelectedExtent2D.height; }

	VkFormat GetVKSwapchainImageFormat() { return m_vkSwapchainImageFormat; }
	uint32_t GetSwapchainImageCount() const { return (uint32_t)m_swapchainImages.size(); }
	
	const VkImageView* GetSwapchainImageViews() { 
		return m_swapchainImageViews.data(); 
	}

	const VkImageView* GetSwapchainImageView(uint32_t index) {
		return &(m_swapchainImageViews[index]);
	}

	const VkFramebuffer GetSwapchainFramebuffers(uint32_t i) const;

	const VkSwapchainKHR GetVKSwapchainHandle() const { return m_vkSwapchain; }

	const uint32_t GetFramebufferCount() const {
		return (uint32_t)(m_vkFramebuffers.size());
	}

	// This will actually create the swapchain
	static EPRef<VKSwapchain> make(
		VkPhysicalDevice vkPhysicalDevice,
		VkSurfaceKHR vkSurface,
		VkDevice vkLogicalDevice,
		VkFormat vkSurfaceFormat,
		VkColorSpaceKHR vkColorSpaceKHR,
		VkPresentModeKHR vkPresentModeKHR,
		VkExtent2D vkExtent2D
	);

	// This just gets the swap chain initialized
	static EPRef<VKSwapchain> make(VkPhysicalDevice vkPhysicalDevice, VkSurfaceKHR vkSurface);

	const EPVector<VkSurfaceFormatKHR>& SurfaceFormats() const { 
		return const_cast<const EPVector<VkSurfaceFormatKHR>&>(m_vkSurfaceFormats); 
	}

	const EPVector<VkPresentModeKHR>& PresentationModes() const { 
		return const_cast<const EPVector<VkPresentModeKHR>&>(m_vkPresentationModes);
	}

private:
	VkSurfaceCapabilitiesKHR m_vkSurfaceCapabilities = {};
	uint32_t m_vkPhysicalDeviceSurfaceFormatCount = 0;
	EPVector<VkSurfaceFormatKHR> m_vkSurfaceFormats;
	VkSurfaceFormatKHR m_vkSelectedSurfaceFormat = {};

	uint32_t m_vkPhysicalDevicePresenationModeCount = 0;
	EPVector<VkPresentModeKHR> m_vkPresentationModes;
	VkPresentModeKHR m_vkSelectedPresentationMode = {};

	VkPhysicalDevice m_vkPhysicalDevice = nullptr;
	VkSurfaceKHR m_vkSurface = nullptr;
	VkDevice m_vkLogicalDevice = nullptr;

	VkExtent2D m_vkSelectedExtent2D;

	VkSwapchainCreateInfoKHR m_vkSwapchainCreateInfo = {};

	VkSwapchainKHR m_vkSwapchain = nullptr;

	uint32_t m_swapchainImageCount = 0;
	VkFormat m_vkSwapchainImageFormat;
	EPVector<VkImage> m_swapchainImages;
	EPVector<VkImageView> m_swapchainImageViews;


// Framebuffers
	EPRef<VKPipeline> m_pVKPipeline = nullptr;
	EPVector<EPRef<VKFramebuffer>> m_vkFramebuffers;
};

#endif // ! VULKAN_SWAPCHAIN_H_