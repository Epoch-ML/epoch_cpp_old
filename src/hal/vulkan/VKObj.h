#ifndef VK_OBJ_H_
#define VK_OBJ_H_

#include "core/ehm/ehm.h"

// epoch Vulkan Object
// epoch/src/hal/vulkan/VKImageView.h

#include <vulkan/vulkan.h>

#include "VulkanUtilities.h"

#include "core/types/EPObj.h"

class VKObj : public EPObj
{
protected:
	VKObj() = default;

	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;

public:
	virtual ~VKObj() = default;
};

#endif // ! VK_OBJ_H_