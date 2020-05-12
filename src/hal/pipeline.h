#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "core/ehm/ehm.h"

// epoch hal pipeline
// epoch/src/hal/pipeline.h

#include "core/types/EPObj.h"

class pipeline :
	public EPObj
{
public:
	pipeline() = default;
	virtual ~pipeline() = default;

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;
	virtual RESULT Update(uint32_t index) = 0;
};

#endif // ! PIPELINE_H_