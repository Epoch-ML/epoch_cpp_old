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

	CRM(projectEuler.m_pfnProblem8(), "This should fail");

Error:
	return 0;
}