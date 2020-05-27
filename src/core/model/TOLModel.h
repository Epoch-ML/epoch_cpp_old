#ifndef TOL_MODEL_H_
#define TOL_MODEL_H_

#include "core/ehm/result.h"

// epoch core tiny obj loader model
// epoch/src/core/model/TOLModel.h

#include "core/types/EPObj.h"
#include "core/types/EPString.h"

#include "model.h"

class TOLModel :
	public model
{
public:
	TOLModel(const EPString<char>& strImageFilename) :
		model(strImageFilename)
	{
		//
	}

	~TOLModel() = default;

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;
};


#endif // ! TOL_MODEL_H_