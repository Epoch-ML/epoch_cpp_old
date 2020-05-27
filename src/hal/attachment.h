#ifndef ATTACHMENT_H_
#define ATTACHMENT_H_

#include "core/ehm/ehm.h"

// epoch hal attachment
// epoch/src/hal/attachment.h

#include "core/types/EPObj.h"

class attachment :
	public EPObj
{
public:
	attachment() = default;
	virtual ~attachment() = default;

public:
	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() = 0;
};

#endif // ! ATTACHMENT_H_