#ifndef TOL_MODEL_H_
#define TOL_MODEL_H_

#include "core/ehm/result.h"

// epoch core tiny obj loader model
// epoch/src/core/model/TOLModel.h

#include "core/types/EPObj.h"
#include "core/types/EPString.h"
#include "core/types/EPFactoryMethod.h"

#include "model.h"

class TOLModel :
	public model,
	virtual public EPFactoryMethod<TOLModel, const EPString<char>&, const EPString<char>&>
{
public:
	TOLModel(const EPString<char>& strModelFilename, const EPString<char>& strTextureFilename) :
		model(strModelFilename, strTextureFilename)
	{
		//
	}

	~TOLModel() = default;

	virtual RESULT Initialize() override;
	virtual RESULT Kill() override;

	static EPRef<TOLModel> InternalMake(const EPString<char>& strModelFilename, const EPString<char>& strTextureFilename);
};


#endif // ! TOL_MODEL_H_