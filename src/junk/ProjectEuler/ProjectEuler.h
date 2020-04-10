#ifndef PROJECT_EULER_H_
#define PROJECT_EULER_H_

#include "core/ehm/ehm.h"

// epoch powered project Euler (Idan Beck) solutions

#include "test/EPTestSuite.h"

#include <functional>

class ProjectEuler : 
	public EPTestSuite 
{
public:
	ProjectEuler() = default;
	~ProjectEuler() = default;

public:
	RESULT Problem8();

	//EPFunction<RESULT()> m_pfnProblem8 = [=]() -> RESULT {	
	//	return this->Problem8();
	//};

	//std::function<RESULT()> m_pfnProblem8 = [=]() -> RESULT {	
	//	return this->Problem8();
	//};

private:
	
};


#endif // ! PROJECT_EULER_H_