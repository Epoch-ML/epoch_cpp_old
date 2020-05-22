#ifndef SAMPLER_H_
#define SAMPLER_H_

#include "core/ehm/ehm.h"

// epoch hal sampler
// epoch/src/hal/sampler.h

#include "core/types/EPObj.h"

class sampler :
	public EPObj
{
public:
	sampler() = default;
	virtual ~sampler() = default;

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;
};

#endif // ! SAMPLER_H_