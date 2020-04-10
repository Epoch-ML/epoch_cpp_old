#include "ProjectEuler.h"

#include <stdio.h>

#include "core/types/EPTuple.h"

int main(int argc, char* argv[]) {
	RESULT r = RESULT::OK;

	DEBUG_LINEOUT("Starting...");

	//ProjectEuler projectEuler; 
	//projectEuler.Run("problem 8");

	tuple<int, int, int> newTuple(5, 10, 15);

	// Test the tuple
	DEBUG_LINEOUT("(%d, %d, %d)", 
		newTuple.get<0, int>(), 
		newTuple.get<1, int>(), 
		newTuple.get<2, int>()
	);

Error:
	return 0;
}