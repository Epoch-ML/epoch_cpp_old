#ifndef VULKAN_PIPELINE_H_
#define VULKAN_PIPELINE_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Pipeline
// epoch/src/hal/vulkan/VKPipeline.h

#include "hal/pipeline.h"

#include <vulkan/vulkan.h>

#include "core/types/EPVector.h"
#include "core/types/EPRef.h"

class VKPipeline :
	public pipeline,
	public EPFactoryMethod
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

};

#endif // ! VULKAN_PIPELINE_H_