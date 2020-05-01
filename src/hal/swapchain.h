#ifndef SWAPCHAIN_H_
#define SWAPCHAIN_H_

#include "core/ehm/ehm.h"

// epoch hal swapchain
// epoch/src/hal/swapchain.h

#include "core/types/EPObj.h"

class swapchain :
	public EPObj
{
public:
	swapchain() = default;
	virtual ~swapchain() = default;

protected:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;

public:
	virtual uint32_t GetExtentsWidth() = 0;
	virtual uint32_t GetExtentsHeight() = 0;
};

#endif // ! SWAPCHAIN_H_