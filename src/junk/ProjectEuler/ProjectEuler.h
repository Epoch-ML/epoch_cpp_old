#ifndef PROJECT_EULER_H_
#define PROJECT_EULER_H_

#include "core/ehm/ehm.h"

// epoch powered project Euler (Idan Beck) solutions

#include "test/EPTestSuite.h"
#include "core/types/EPFunction.h"

class ProjectEuler : 
	public EPTestSuite<ProjectEuler>
{
public:
	ProjectEuler() = default;
	~ProjectEuler() = default;

public:
	RESULT Problem8();

	EPFunction<RESULT()> m_pfnProblem8 = [&]() -> RESULT {	
		return this->Problem8();
	};

	// TODO: Switch this to system in types test suite
	virtual RESULT AddTests() override {
		return R::NOT_IMPLEMENTED;
	}

private:
	
};


#endif // ! PROJECT_EULER_H_