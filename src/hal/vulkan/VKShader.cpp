#include "VKShader.h"

#include <fstream>

#include "VulkanUtilities.h"

RESULT VKShader::Initialize() {
	RESULT r = R::OK;
	EPVector<char> buffer;
	size_t fileShader_n = 0;
	std::ifstream fileShader;

	CNM(m_vkLogicalDevice, "Cannot create shader without valid logical device");

	m_strFilenamePath = EPString<char>(EPOCH_SHADERS_DIR) + "/" + m_strFilename;

	fileShader = std::ifstream(m_strFilenamePath.c_str(), std::ios::ate | std::ios::binary);
	CBM(fileShader.is_open(), "Failed to open %s", m_strFilename.c_str());

	// ingest file contents
	fileShader_n = (size_t)fileShader.tellg();
	buffer = EPVector<char>(fileShader_n, true);
	fileShader.seekg(0);
	fileShader.read(buffer.data(), fileShader_n);

	fileShader.close();
	CBM(fileShader.is_open() == false, "Failed to close %s", m_strFilename.c_str());

	// Set up the shader module

	m_vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	m_vkShaderModuleCreateInfo.codeSize = buffer.size();
	m_vkShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

	CVKRM(vkCreateShaderModule(m_vkLogicalDevice, &m_vkShaderModuleCreateInfo, nullptr, &m_vkShaderModule),
		"Failed to create Shader module");

Error:
	return r;
}

RESULT VKShader::Kill() {
	RESULT r = R::OK;

	CN(m_vkLogicalDevice);

	vkDestroyShaderModule(m_vkLogicalDevice, m_vkShaderModule, nullptr);
	m_vkShaderModule = nullptr;

Error:
	return r;
}

EPRef<VKShader> VKShader::InternalMake(const EPString<char> &strFilename, VkDevice vkLogicalDevice) {
	RESULT r = R::OK;
	EPRef<VKShader> pVKShader = nullptr;

	pVKShader = new VKShader(strFilename, vkLogicalDevice);
	CNM(pVKShader, "Failed to allocate shader");

	CRM(pVKShader->Initialize(), "Failed to initialize VK shader");

Success:
	return pVKShader;

Error:
	pVKShader = nullptr;
	return nullptr;
}