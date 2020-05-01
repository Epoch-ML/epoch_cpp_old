#ifndef VULKAN_SHADER_H_
#define VULKAN_SHADER_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Shader
// epoch/src/hal/vulkan/VKShader.h

#include "hal/shader.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"
#include "core/types/EPString.h"

class VKShader :
	public shader,
	public EPFactoryMethod<VKShader, const EPString<char>&, VkDevice>
{
private:
	VKShader(const EPString<char> &strFilename, VkDevice vkLogicalDevice) :
		m_strFilename(strFilename),
		m_vkLogicalDevice(vkLogicalDevice)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKShader() override {
		Kill();
	}

	static EPRef<VKShader> InternalMake(const EPString<char>&, VkDevice);

private:
	EPString<char> m_strFilename;
	EPString<char> m_strFilenamePath;

	VkDevice m_vkLogicalDevice = nullptr;

	VkShaderModuleCreateInfo m_vkShaderModuleCreateInfo = {};
	VkShaderModule m_vkShaderModule = nullptr;
};

#endif // ! VULKAN_PIPELINE_H_