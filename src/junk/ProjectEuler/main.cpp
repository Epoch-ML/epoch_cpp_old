#include "ProjectEuler.h"

#include <stdio.h>

#include "core/types/EPTuple.h"
#include "core/types/EPFunction.h"

int main(UNUSED int argc, UNUSED char* argv[]) {
	RESULT r = RESULT::OK;

	DEBUG_LINEOUT("Starting...");

	ProjectEuler projectEuler; 
	//projectEuler.Run("problem 8");

	CRM(projectEuler.m_pfnProblem8(), "This should fail");

Error:
	return 0;
}