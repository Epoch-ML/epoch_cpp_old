#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "core/ehm/ehm.h"

// epoch hal framebuffer
// epoch/src/hal/framebuffer.h

#include "core/types/EPObj.h"

class framebuffer :
	public EPObj
{
public:
	framebuffer() = default;
	virtual ~framebuffer() = default;

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;
};

#endif // ! FRAMEBUFFER_H_