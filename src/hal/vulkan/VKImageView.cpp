#include "VKImageView.h"

RESULT VKImageView::Initialize() {
	RESULT r = R::OK;



Error:
	return r;
}

RESULT VKImageView::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);

	// Free memory

Error:
	return r;
}

EPRef<VKImageView> VKImageView::InternalMake(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice
) {
	RESULT r = R::OK;
	EPRef<VKImageView> pVKImageView = nullptr;

	pVKImageView = new VKImageView(
		vkPhysicalDevice,
		vkLogicalDevice
	);
	CNM(pVKImageView, "Failed to allocate vk image view");

	CRM(pVKImageView->Initialize(), "Failed to initialize VK image view");

Success:
	return pVKImageView;

Error:
	pVKImageView = nullptr;
	return nullptr;
}

VKImageView::VKImageView(
	VkPhysicalDevice vkPhysicalDevice,
	VkDevice vkLogicalDevice
) :
	m_vkPhysicalDevice(vkPhysicalDevice),
	m_vkLogicalDevice(vkLogicalDevice)
{
	//
}

VKImageView::~VKImageView() {
	Kill();
}