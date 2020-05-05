#ifndef SHADER_H_
#define SHADER_H_

#include "core/ehm/ehm.h"

// epoch hal shader
// epoch/src/hal/shader.h

#include "core/types/EPObj.h"

class shader :
	public EPObj
{
public:
	shader() = default;
	virtual ~shader() = default;

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;
};

#endif // ! SHADER_H_