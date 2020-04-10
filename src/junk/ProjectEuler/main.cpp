#include "ProjectEuler.h"

#include <stdio.h>

#include "core/types/EPTuple.h"
#include "core/types/EPFunction.h"

int main(int argc, char* argv[]) {
	RESULT r = RESULT::OK;

	DEBUG_LINEOUT("Starting...");

	ProjectEuler projectEuler; 
	//projectEuler.Run("problem 8");

	EPFunction<RESULT()> m_pfnProblem8(
	[]() -> RESULT {
			return R::OK;;
	});

Error:
	return 0;
}