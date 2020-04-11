#include "ProjectEuler.h"

#include <stdio.h>

#include "core/types/EPTuple.h"
#include "core/types/EPFunction.h"

int main(int argc, char* argv[]) {
	RESULT r = RESULT::OK;

	int a = 5;

	DEBUG_LINEOUT("Starting...");

	ProjectEuler projectEuler; 
	//projectEuler.Run("problem 8");

	EPFunction<RESULT(int b)> m_pfnProblem8 = 
	[&](int b) -> RESULT {
		if(a == b)
			return R::OK;
		else
			return R::FAIL;
	};

	CRM(m_pfnProblem8(7), "This should fail");

Error:
	return 0;
}