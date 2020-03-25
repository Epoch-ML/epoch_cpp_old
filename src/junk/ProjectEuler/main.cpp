#include "ProjectEuler.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
	RESULT r = RESULT::OK;

	DEBUG_LINEOUT("Starting...");

	ProjectEuler projectEuler; 
	projectEuler.Run("problem 8");

Error:
	return 0;
}