#ifndef COMMAND_BUFFER_H_
#define COMMAND_BUFFER_H_

#include "core/ehm/ehm.h"

// epoch hal command buffer
// epoch/src/hal/command_buffer.h

#include "core/types/EPObj.h"

class command_buffer :
	public EPObj
{
public:
	command_buffer() = default;
	virtual ~command_buffer() = default;

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;
};

#endif // ! COMMAND_BUFFER_H_