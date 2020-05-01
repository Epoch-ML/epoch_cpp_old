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
	public EPFactoryMethod<VKShader, const EPString<char>&>
{
private:
	VKShader(const EPString<char> &strFilename) :
		m_strFilename(strFilename)
	{
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKShader() override {
		Kill();
	}

	static EPRef<VKShader> InternalMake(const EPString<char>&);

private:
	EPString<char> m_strFilename;
	EPString<char> m_strFilenamePath;
};

#endif // ! VULKAN_PIPELINE_H_