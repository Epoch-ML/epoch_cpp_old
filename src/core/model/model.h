#ifndef MODEL_H_
#define MODEL_H_

#include "core/ehm/result.h"

// epoch core model
// epoch/src/core/model/model.h

#include "core/types/EPObj.h"
#include "core/types/EPString.h"

class model :
	public EPObj
{
public:

	// TODO: Is this the best way
	model() = default;

	model(const EPString<char>& strImageFilename) :
		m_strImageFilename(strImageFilename)
	{}

	~model() = default;

	virtual RESULT Initialize() = 0;
	virtual RESULT Kill() { return R::NOT_IMPLEMENTED; }

protected:
	EPString<char> m_strImageFilename;
};


#endif // ! MODEL_H_