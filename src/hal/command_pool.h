#ifndef COMMAND_POOL_H_
#define COMMAND_POOL_H_

#include "core/ehm/ehm.h"

// epoch hal command pool
// epoch/src/hal/command_pool.h

#include "core/types/EPObj.h"

class command_pool :
	public EPObj
{
public:
	command_pool() = default;
	virtual ~command_pool() = default;

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;
};

#endif // ! COMMAND_POOL_H_