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
	public EPFactoryMethod<VKShader, VkDevice, const EPString<char>&, VkShaderStageFlagBits>
{
private:
	VKShader(const EPString<char>& strFilename, VkDevice vkLogicalDevice, VkShaderStageFlagBits vkShaderStageFlagBits);

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKShader() override;

	const VkPipelineShaderStageCreateInfo &GetShaderStageCreateInfo() {
		return m_vkPipelineShaderStateCreateInfo;
	}

	static EPRef<VKShader> InternalMake(VkDevice, const EPString<char>&, VkShaderStageFlagBits);

private:
	EPString<char> m_strFilename;
	EPString<char> m_strFilenamePath;

	VkDevice m_vkLogicalDevice = nullptr;

	VkShaderModuleCreateInfo m_vkShaderModuleCreateInfo = {};
	VkShaderModule m_vkShaderModule = nullptr;
	VkShaderStageFlagBits m_vkShaderStageFlagBits;

	// TODO: This should be spun out into a different object likely
	VkPipelineShaderStageCreateInfo m_vkPipelineShaderStateCreateInfo = {};

};

#endif // ! VULKAN_PIPELINE_H_