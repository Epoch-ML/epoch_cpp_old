#ifndef BUFFER_H_
#define BUFFER_H_

#include "core/ehm/ehm.h"

// epoch hal buffer
// epoch/src/hal/buffer.h

// TODO: How is this different than framebuffer - 
// should we merge them?

#include "core/types/EPObj.h"

template <typename TStorage> class EPVector;

class buffer :
	public EPObj
{
public:
	buffer() = default;
	virtual ~buffer() = default;

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;
	virtual RESULT Bind() = 0;
};

#endif // ! BUFFER_H_