#ifndef PROJECT_EULER_H_
#define PROJECT_EULER_H_

#include "core/ehm/ehm.h"

// epoch powered project Euler (Idan Beck) solutions

#include "test/EPTestSuite.h"

class ProjectEuler : 
	public EPTestSuite 
{
public:
	ProjectEuler() = default;
	~ProjectEuler() = default;


public:
	RESULT Problem8();

	//EPFunctionPack<RESULT()> m_pfnProblem8 = 
	//[]() -> RESULT {	
	//	return Problem8();
	//};

private:
	
};


#endif // ! PROJECT_EULER_H_