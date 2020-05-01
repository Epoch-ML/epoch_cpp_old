#ifndef VULKAN_PIPELINE_H_
#define VULKAN_PIPELINE_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Pipeline
// epoch/src/hal/vulkan/VKPipeline.h

#include "hal/pipeline.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"
#include "core/types/EPFactoryMethod.h"

#include "VKShader.h"

class VKPipeline :
	public pipeline,
	public EPFactoryMethod<VKPipeline>
{
private:
	VKPipeline()  {
		//
	}

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

public:
	virtual ~VKPipeline() override {
		Kill();
	}

	static EPRef<VKPipeline> InternalMake();

private:
	EPRef<VKShader> m_pVertexShader = nullptr;
	EPRef<VKShader> m_pFragmentShader = nullptr;
};

#endif // ! VULKAN_PIPELINE_H_