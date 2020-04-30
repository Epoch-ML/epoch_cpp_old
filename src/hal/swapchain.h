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
	swapchain() = delete;
	virtual ~swapchain() = 0;

public:
	virtual RESULT Initialize() = 0;
};

#endif // ! SWAPCHAIN_H_